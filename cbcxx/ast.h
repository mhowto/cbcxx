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

    ExtDeclList extDecls_;
};

#endif
