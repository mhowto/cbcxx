#ifndef CBCXX_VISITOR_H
#define CBCXX_VISITOR_H

class BinaryOp;
class UnaryOp;
class ConditionalOp;
class FuncCall;
class Identifier;
class Object;
class Enumerator;
class Constant;
class TempVar;

class Declaration;
class IfStatement;
class JumpStatement;
class ReturnStatement;
class LabelStatement;
class EmptyStatement;
class CompoundStatment;
class FuncDef;
class TranslationUnit;

class Visitor {
public:
    virutal ~Visitor() {}
    virutal void Visit(BianryOp* binary) = 0;
    virtual void Visit(UnaryOp* unary) = 0;
    virtual void Visit(ConditionalOp* cond) = 0;
    virtual void Visit(FuncCall* func) = 0;
    virtual void Visit(Identifier* iden) = 0;
    virtual void Visit(Object* obj) = 0;
    virtual void Visit(Enumerator* en) = 0;
    virtual void Visit(Constant* con) = 0;
    virtual void Visit(TempVar* temp) = 0;

    virtual void Visit(Declaration* decl) = 0;
    virtual void Visit(IfStatement* ifStmt) = 0;
    virtual void Visit(JumpStatement* jumpStmt) = 0;
    virtual void Visit(ReturnStatement* returnStmt) = 0;
    virtual void Visit(LabelStatement* labelStmt) = 0;
    virtual void Visit(EmptyStatement* emptyStmt) = 0;
    virtual void Visit(CompoundStatement* compoundStmt) = 0;
    virtual void Visit(FuncDef* funcDef) = 0;
    virtual void Visit(TranslationUnit* unit) = 0;
};

#endif