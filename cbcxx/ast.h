#ifndef CBCXX_AST_H
#define CBCXX_AST_H

#include <list>
#include <string>

class Visitor;
class Identifier;

class LabelStmt;
class CompoundStmt;


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

/* Statements End*/

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

    ExtDeclList extDecls_;
};

#endif
