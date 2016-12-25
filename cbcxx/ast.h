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
    virtual void Accept(Visitor* v) = 0; // 访问者模式

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
    int tag_; // 使用整型的tag值, Parser中保存label映射表(string->labelStmt)
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
    LabelStatement* label_; // 要跳转到的地方
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
    1. 奇葩的concat string literal n1548 6.4.5 #9
       char c[] = "a" L"b" "c"; ==> char c[] = L"abc";
    2. Array Literal
        char c[] = {'a', 'b', 'c'};
    3. 奇葩的数组初始化 6.7.9 #36 也不奇葩，其实是C99的重要特性 指定初始化器 designated initializer
        int a[MAX] = {
            1,3,5,7,9,[MAX-5]=8,6,4,2,0
        };
    4. 同3类似，还有struct initilizator
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

    Type* type_;  // 该Initializer返回的是什么类型
    int offset_;  // 主要用于array initializer中的 指定初始化器情况 表明这个初始化的便宜是多少
                  // offset_ / arraytpe.width = index
    unsigned char bitFieldBegin_; // struct initilizator 情况下, 用于标识这个域在struct的位域
    unsigned char bitFieldWidth_;
    Expr* expr_;
};

typedef std::set<Initializer> InitList;

/*
    declaration 与 definition的区别
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
    virtual bool IsLVal() = 0;  // 是否左值
    virtual void TypeChecking() = 0;
    void EnsureCompatible(const QualType lhs, const QualType rhs) const;
    void EnsureCompatibleOrVoidPointer(const QualType lhs, const QualType rhs) const;

    const Token* Tok() const { return tok_; }
    void SetToken(const Token* tok) { tok_ = tok; }

    static Expr* MayCast(Expr* expr);  // 尝试类型转换
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
    Identifier* ident_; // 函数名字
    LabelStmt* retLabel_; // 标识了return语句返回的地方，CodeGen时候有用
    CompoundStmt* body_; // 函数体
};

using ExtDeclList = std::list<ExtDecl*>;

// 开始的地方 
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

    ExtDeclList extDecls_;  // 存放变量声明，函数声明等等
};

#endif
