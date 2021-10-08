#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <set>

using namespace std;

class FuncElements {
public:
    vector<string> dequeVars; // проверяет очерёдность типов переменных
    vector<string> dequeArgs; // проверяет очерёдность типов аргументов

    map<string, int> localInts; // имя + значение локальной переменной int
    map<string, char> localChars; // имя + значение локальной переменной char
    map<string, short> localShorts; // имя + значение локальной переменной short

    map<string, int> argInts; // имя + значение аргумента int
    map<string, char> argChars; // имя + значение аргумента char
    map<string, short> argShorts; // имя + значение аргумента short

    map<string, int> forIterators; // имя + значение итератора для конструкции FOR
};

class Parser {
private:
    int curFor; // для проверки, находится ли парсер в конструкции FOR и если да, то в какой вложенности конструкции
    int curIf; // для проверки, находится ли парсер в конструкции IF и если да, то в какой вложенности конструкции
    bool inFunc; // для проверки, находится ли парсер в функции (любой)
    bool inArgs; // для проверки, будут ли сейчас парситься аргументы функции
    bool isElse; // для проверки, является ли следующий код конструкцией else {...} 
    string curFunc; // если парсер в функции, то curFunc - название этой функции

    vector<string> logicCommands; // пропарсенная запись 

    map<string, char> globalVarChar; // глоб. переменные char
    map<string, int> globalVarInt; // глоб. переменные int
    map<string, short> globalVarShort; // глоб. переменные short
    set<string> constStrings; // глоб. строки

    map<string, FuncElements> funcs; // имя функции + состав функции

    template <typename T>
    bool checkVar(ifstream& stream, const T& forParse, const string& current) {
        bool tmp = false;
        for (const auto& item : forParse) {
            if (item.first == current) {
                tmp = true;
                string checkOP;
                stream >> checkOP;
                if (checkOP == "=")
                    ParsMath(stream, current);
                else
                    thtow exception();
            }
        }
        return tmp;
    }

    bool CheckVars(ifstream& stream, const string& current) {
        if (checkVar(stream, globalVarChar, current) || checkVar(stream, globalVarInt, current) || checkVar(stream, globalVarShort, current))
        {
            return true;
        }
        return false;
    }

    void CheckConstruction(ifstream& stream) {
        if (stream.eof())
            return;

        string current;
        stream >> current;

        /*
         if (current == "int" || current == "char" || current == "short") // createVar
        {
            ParsVars(stream, current);
        }
        else if (current == "if") // if
        {
            isElse = false;
            ParsCondition(stream);
        }
        else if (current == "else") // else
        {
            isElse = true;
            ParsCondition(stream);
        }
        else if (current == "for") // for
        {
            ParsFor(stream);
        }
        else if (current[0] == '\"') // string
        {
            ParsStr(stream, current);
        }
        else if (current == "func") // func
        {
            ParsFunc(stream);
        }
        else if (current == "}") // end func/if/for
        {
            return true;
        }
        else if (current == "print") // print
        {
            ParsPrint(stream);
        }
        else
        {
            if (CheckVars(stream, current))
                return;

            for (const auto& item : funcs) {
                if (item.first == current) {
                    ParsCall(stream, current);
                    return;
                }
            }

            throw exception();
        }*/
    }
    void ParsPrint(ifstream& stream);
    void ParsMath(ifstream& stream, const string& varToMath);
    void ParsVars(ifstream& stream, const string& typeVar);
    void ParsCondition(ifstream& stream);
    void ParsFor(ifstream& stream);
    void ParsStr(ifstream& stream, const string& beginStr);
    void ParsFunc(ifstream& stream);
    void ParsCall(ifstream& stream, const string& nameFunc);


    void WriteLogic(string file_with_code) {
        //create/open file.logic
        //write step-by-step logic in file
        //close file.logic
    }

public:
    Parser(string file_with_code) : inArgs(false), inFunc(false), curFor(0), curIf(0) {
        ifstream file;
        file.open(file_with_code);
        if (file.is_open() == 0) { throw exception(); };
        while (file.is_open() == 1) {
            CheckConstruction(file);
        }
        file.close();
        WriteLogic(file_with_code);
    }
};
/*
class UsageReg {
private:
    bool EAX; // используется ли регистр EAX
    bool EBX; // используется ли регистр EBX
    bool ECX; // используется ли регистр ECX
    bool EDX; // используется ли регистр EDX
    bool ESI; // используется ли регистр ESI
    bool EDI; // используется ли регистр EDI

    void changeStatusReg(int numberOfReg);

public:
    string takeFreeReg();

    UsageReg() : EAX(0), EBX(0), ECX(0), EDX(0), ESI(0), EDI(0) {}
};

class Translater {
public:
    Translater(std::string logic_file) : inFunc(false), curFor(0), curIf(0) {
        UsageReg reg;
        //open file.logic
        //check is_file_opened
        //while(file opened)
        //call CheckConstruction()
        //end_while
        //close file.logic
    }
private:
    int curFor; // для проверки, находится ли парсер в конструкции FOR и если да, то в какой вложенности конструкции
    int curIf; // для проверки, находится ли парсер в конструкции IF и если да, то в какой вложенности конструкции
    bool inFunc; // для проверки, находится ли парсер в функции (любой)
    string curFunc; // если парсер в функции, то curFunc - название этой функции

    vector<string> arithm{"+","-","*","/"};
    vector<string> arithmSelf{ "+=", "-=", "*=", "/=", "++", "--"};

    map<string, char> globalVarChar;
    map<string, int> globalVarInt;
    map<string, short> globalVarShort;
    map<string, string> constStrings;
    map<string, FuncElements> funcs;

    vector<string> logicCommands;

    template <typename T>
    bool checkVar(ifstream& stream,const T& forParse, const string& current) {
        bool tmp = false;
        for (const auto& item : forParse) {
            if (item.first == current) {
                tmp = true;
            }
        }
        return tmp;
    }
    bool CheckVars(ifstream& stream, const string& current) {
        if (checkVar(stream, globalVarChar, current) || checkVar(stream, globalVarInt, current) || checkVar(stream, globalVarShort, current))
        {
            return true;
        }
        return false;
    }

    void CheckConstruction(ifstream& stream) {
        if (stream.eof())
            return;

        string current;
        stream >> current;

        if (1) { // if

        }
        else if (1) { // for

        }
        else if (1) { // $

        }
        else if (1) { // call

        }
        else if (current.at(0) == '#') { // #
            //#GlobalVars
            //#ConstSrings
            //#Funcs
        }
        else { // other
            if (CheckVars(stream, current))
                return;

            for (const auto& item : funcs) {
                if (item.first == current) {

                }
            }

            throw exception();
        }
    }
    void TransStackWork(const bool& beginEnd); // prolog, epilog
    void TransGlobVars(ifstream& stream); //#GlobalVars
    void TransVars(ifstream& stream); //vars,args in funcs
    void TransConstStr(ifstream& stream); // "some str"
    void TransFor(ifstream& stream); // @beginf ... @endf
    void TransCondition(ifstream& stream); // if: ? ... ? ... !   else: ?? ... !
    void TransFunc(ifstream& stream); // $ name_func @begin name_func ... @end name_func

    void WriteLogic(){
        //create/open file.asmobj
        //write step-by-step asm commands in file
        //close file.asmobj
    }
};
*/
int main()
{
    cout << "OK!" << endl;
    return 0;
}
