#ifndef CBCXX_AST_H
#define CBCXX_AST_H

#include <list>
#include <string>

#include "type.h"

class Visitor;
class Identifier;

class LabelStmt;
class CompoundStmt;
class Expr;
class Scope;
class Object;
class Type;
class QualType;
class Token;


class ASTNode {
public:
    virtual ~ASTNode() {}  // effective c++ Item07
    virtual void Accept(Visitor* v) = 0; // ������ģʽ

protected:
    ASTNode() {}
};

typedef ASTNode ExtDecl;

/* Statements Begin*/
class Statement : public ASTNode {
public:
    virtual ~Statement() {}

protected:
    Statement() {}
};

/* EmptyStatement like
    int main() {
    {
      // now this is empty statement
    }
*/
class EmptyStatement : public Statement {
public:
    static EmptyStatement* New();
    virtual ~EmptyStatement() {}
    virtual void Accept(Visitor* v);
protected: 
    EmptyStatement() {}
};

class LabelStatement : public Statement {
public:
    static LabelStatement* New();
    ~LabelStatement() {}
    virtual void Accept(Visitor* v);
    std::string Repr() const { return ".L" + std::to_string(tag_); } // code_gen

protected:
    LabelStatement(): tag_(GenTag()) {}

private:
    static int GenTag() {
        static int tag = 0;
        return ++tag;
    }
    int tag_; // ʹ�����͵�tagֵ, Parser�б���labelӳ���(string->labelStmt)
};

/*
    if <cond_expr> {
        then statement
    } [else {
        else statement
    }]
*/
class IfStatement : public Statement {
public:
    static IfStatement* New(Expr* cond, Statement* then, Statement* els = nullptr);
    virtual ~IfStatement() {}
    virtual void Accept(Visitor* v);

protected:
    IfStatement(Expr* cond, Statement* then, Statement* els = nullptr)
        : cond_(cond), then_(then), else_(els) {}

private:
    Expr* cond_;
    Statement* then_;
    Statement* else_;
};

/*
    goto label
    break ..
    continue ..
*/
class JumpStatement : public Statement {
public:
    static JumpStatement* New(LabelStatement* label);
    virtual ~JumpStatement() {}
    virtual void Accept(Visitor* v);
    void SetLabel(LabelStatement* label) { label_ = label; }

protected:
    JumpStatement(LabelStatement* label): label_(label) {}
private:
    LabelStatement* label_; // Ҫ��ת���ĵط�
};

/*
    return expr;
    return;
*/
class ReturnStatment : public Statement {
public:
    static ReturnStatment* New(Expr* expr);
    virtual ~ReturnStatment() {}
    virtual void Accept(Visitor* v);

protected:
    ReturnStatment(Expr* expr): expr_(expr) {}
private:
    Expr* expr_;
};

typedef std::list<Statement*> StatementList;

/*
    {
        statement1
        statement2
        ...
    }
*/
class CompoundStatement : public Statement {
public:
    static CompoundStatement* New(StatementList& stmts, Scope* scope = nullptr);
    virtual ~CompoundStatement() {}
    virtual void Accept(Visitor* v);
    StatementList& Stmts() { return stmts_; }
    Scope* Scope() { return scope_; }
protected:
    CompoundStatement(const StatementList& stmts, ::Scope* scope=nullptr)
        : stmts_(stmts), scope_(scope) {}
private:
    StatementList stmts_;
    ::Scope* scope_;
};

/*
    for semantic use
    1. �����concat string literal n1548 6.4.5 #9
       char c[] = "a" L"b" "c"; ==> char c[] = L"abc";
    2. Array Literal
        char c[] = {'a', 'b', 'c'};
    3. ����������ʼ�� 6.7.9 #36 Ҳ�����⣬��ʵ��C99����Ҫ���� ָ����ʼ���� designated initializer
        int a[MAX] = {
            1,3,5,7,9,[MAX-5]=8,6,4,2,0
        };
    4. ͬ3���ƣ�����struct initilizator
        struct {int a[3], b;} w[] = { [0].a = {1}, [1].a[0] = 2};
*/
struct Initializer {
    Initializer(Type* type, int offset, Expr* expr, unsigned char bitFieldBegin=0, unsigned char bitFieldWidth=0)
        :   type_(type),
        offset_(offset),
        bitFieldBegin_(bitFieldBegin),
        bitFieldWidth_(bitFieldWidth),
        expr_(expr) {}

    bool operator<(const Initializer& rhs) const;

    Type* type_;  // ��Initializer���ص���ʲô����
    int offset_;  // ��Ҫ����array initializer�е� ָ����ʼ������� ���������ʼ���ı����Ƕ���
                  // offset_ / arraytpe.width = index
    unsigned char bitFieldBegin_; // struct initilizator �����, ���ڱ�ʶ�������struct��λ��
    unsigned char bitFieldWidth_;
    Expr* expr_;
};

typedef std::set<Initializer> InitList;

/*
    declaration �� definition������
*/
class Declaration : public Statement {
public:
    static Declaration* New(Object* obj);
    virtual ~Declaration() {}
    virtual void Accept(Visitor* v);
    InitList& Inits() { return inits_; }
    Object* Obj() { return obj_; }
    void AddInit(Initializer init);

protected:
    Declaration(Object* obj): obj_(obj) {}
private:
    Object* obj_;
    InitList inits_;
};

/* Statements End*/

/*Expr:
    BinaryOp
    UnaryOp
    ConditionalOp
    FuncCall
    Constant
    Identifier
    Object
    TempVar
*/

class Expr : public Statement {
public:
    virtual ~Expr() {}
    //::Type* Type() { return type_.GetPrt(); }
    virtual bool IsLVal() = 0;  // �Ƿ���ֵ
    virtual void TypeChecking() = 0;
    void EnsureCompatible(const QualType lhs, const QualType rhs) const;
    void EnsureCompatibleOrVoidPointer(const QualType lhs, const QualType rhs) const;

    const Token* Tok() const { return tok_; }
    void SetToken(const Token* tok) { tok_ = tok; }

    static Expr* MayCast(Expr* expr);  // ��������ת��
    static Expr* MayCast(Expr* expr, QualType desType);
    virtual bool IsNullPointerConstant() const { return false; }
    bool IsConstQualified() const { return type_.IsConstQualified(); }
    bool IsRestrictQualified() const { return type_.IsRestrictQualified(); }
    bool IsVolatileQualified() const { return type_.IsVolatileQualified(); }

private:
    const Token* tok_;
    QualType type_;
};


/* Expr End*/

class FuncDef : public ASTNode {
public:
    static FuncDef* New(Identifier* ident, LabelStmt* retLabel);
    virtual ~FuncDef() {}
    virtual void Accept(Visitor* v);

    CompoundStmt* Body() { return body_; }
    void SetBody(CompoundStmt* body) { body_ = body; }
    std::string Name() const { return "ident_->Name()"; }

protected:
    FuncDef(Identifier* ident, LabelStmt* retLabel) :
        ident_(ident), retLabel_(retLabel) {}

private:
    Identifier* ident_; // ��������
    LabelStmt* retLabel_; // ��ʶ��return��䷵�صĵط���CodeGenʱ������
    CompoundStmt* body_; // ������
};

using ExtDeclList = std::list<ExtDecl*>;

// ��ʼ�ĵط� 
class TranslationUnit : public ASTNode {
public:
    static TranslationUnit* New() { return new TranslationUnit(); }
    virtual ~TranslationUnit() {}
    virtual void Accept(Visitor* v);

    void Add(ExtDecl* extDecl) { extDecls_.push_back(extDecl); }
    ExtDeclList& ExtDecls() { return extDecls_; }
    const ExtDeclList& ExtDecls() const { return extDecls_; }

private:
    TranslationUnit() {}

    ExtDeclList extDecls_;  // ��ű������������������ȵ�
};

#endif
