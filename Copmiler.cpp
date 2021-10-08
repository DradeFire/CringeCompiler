﻿#include <iostream>
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
};


class Parser {
public:
    int curFor; // для проверки, находится ли парсер в конструкции FOR и если да, то в какой вложенности конструкции
    int curIf; // для проверки, находится ли парсер в конструкции IF и если да, то в какой вложенности конструкции
    bool inFunc; // для проверки, находится ли парсер в функции (любой)
    string curFunc="Функция"; // если парсер в функции, то curFunc - название этой функции
    bool inArgs= 0;  // для проверки, находится ли парсер в параметрах функции 

    string typeWar = "short";//для проверки ParsVars

    vector<string> logicCommands;

    map<string, char> globalVarChar;
    map<string, int> globalVarInt;
    map<string, short> globalVarShort;
    set<string> constStrings;

    map<string, FuncElements> funcs;
   
    void  Forbidden_symbols_and_name(string nameVar) {

        vector<char> forbidden_symbols = { '!','@','"','#','№','$',';','^',':','&','?','*','<','>','`','~','/','|','\'','\\','.',',','+','-','%','=' };

        for (auto forbsimb : forbidden_symbols) {

            if (nameVar.find(forbsimb) != -1) { throw exception(); };
        }
         
        
        {auto it  = this->globalVarChar.find(nameVar);
        if (it != this->globalVarChar.end()) { throw exception(); }; }
        {auto it = this->globalVarInt.find(nameVar);
        if (it != this->globalVarInt.end()) { throw exception(); }; }
        {auto it = this->globalVarShort.find(nameVar);
        if (it != this->globalVarShort.end()) { throw exception(); }; }
        {auto it = this->funcs.at(curFunc).argInts.find(nameVar);
        if (it != this->funcs.at(curFunc).argInts.end()) { throw exception(); }; }
        {auto it = this->funcs.at(curFunc).argChars.find(nameVar);
        if (it != this->funcs.at(curFunc).argChars.end()) { throw exception(); }; }
        {auto it = this->funcs.at(curFunc).argShorts.find(nameVar);
        if (it != this->funcs.at(curFunc).argShorts.end()) { throw exception(); }; }
        {auto it = this->funcs.at(curFunc).localChars.find(nameVar);
        if (it != this->funcs.at(curFunc).localChars.end()) { throw exception(); }; }
        {auto it = this->funcs.at(curFunc).localShorts.find(nameVar);
        if (it != this->funcs.at(curFunc).localShorts.end()) { throw exception(); }; }
        {auto it = this->funcs.at(curFunc).localInts.find(nameVar);
        if (it != this->funcs.at(curFunc).localInts.end()) { throw exception(); }; }
    }
    void writing_arguments(string nameVar, int value, string typeVar) {
        if ((this->inFunc == 0) && (this->inArgs == 0)) {// записu global переменной в LogicCommands нету

            globalVarInt[nameVar] = value;
        }
        if ((this->inFunc == 0) && (this->inArgs == 1)) {// запись аргумента в LogicCommands нету
            funcs.at(curFunc).argInts[nameVar] = value;
            funcs.at(curFunc).dequeArgs.push_back(typeVar);
        }
        if ((this->inFunc == 1) && (this->inArgs == 0)) {
            funcs.at(curFunc).localInts[nameVar] = value;
            funcs.at(curFunc).dequeVars.push_back(typeVar);
            this->logicCommands.push_back(nameVar + " = " + to_string(value));// запись локальной переменной в LogicCommands
        }
        
    }
    void writing_arguments(string nameVar, char value1, string typeVar) {
        
        char value = static_cast<int>(value1);
        if ((this->inFunc == 0) && (this->inArgs == 0)) {
            
            globalVarChar[nameVar]= value;
        }
         if ((this->inFunc == 0) && (this->inArgs == 1)) {
            funcs.at(curFunc).argChars[nameVar] = value;
            funcs.at(curFunc).dequeArgs.push_back(typeVar);
        }
         if ((this->inFunc == 1) && (this->inArgs == 0)) {
            
             funcs.at(curFunc).localChars[nameVar] = value;
             funcs.at(curFunc).dequeVars.push_back(typeVar);
             this->logicCommands.push_back(nameVar + " = " + to_string(value));
         };
        
    }
    void writing_arguments(string nameVar, short value, string typeVar) {
       
        if ((this->inFunc == 0) && (this->inArgs == 0)) {

            globalVarShort[nameVar]= value;
        }
        if ((this->inFunc == 0) && (this->inArgs == 1)) {
            funcs.at(curFunc).argShorts[nameVar] = value;
            funcs.at(curFunc).dequeArgs.push_back(typeVar);
        }
        if ((this->inFunc == 1) && (this->inArgs == 0)) {
            funcs.at(curFunc).localShorts[nameVar] = value;
            funcs.at(curFunc).dequeVars.push_back(typeVar);
        }
        this->logicCommands.push_back(nameVar + " = " + to_string(value));
        
    }


    void ParsVars(ifstream& stream, const string& typeVar) {
        if ((this->inFunc == 1) && (this->inArgs == 1)) { throw exception(); };
        string nameVar, endCommand, word;
        int int_value = 0; char char_value = ' ';  short short_value = 0;
        stream >> word;


        if (inArgs == 1) {
            auto comma = word.rfind(",");
            auto skoba = word.rfind(")");
            if (skoba != -1) {
                if (word.length() == skoba + 1) {
                    nameVar = word.erase(skoba);
                    if ((nameVar.find(")") != -1) || (nameVar == "")) {
                        throw exception();
                    }
                    endCommand = ")";
                    Forbidden_symbols_and_name(nameVar);

                    if (typeVar == "int") { writing_arguments(nameVar, int_value, typeVar); }
                    if (typeVar == "short") { writing_arguments(nameVar, short_value, typeVar); }
                    if (typeVar == "char") { writing_arguments(nameVar, char_value, typeVar); }
                }
                else { throw exception(); };

            }
            else if (comma != -1) {
                if (word.length() == comma + 1) {
                    nameVar = word.erase(comma);
                    if ((nameVar.find(",") != -1) || (nameVar == "")) {
                        throw exception();
                    }
                    endCommand = ",";
                    Forbidden_symbols_and_name(nameVar);
                    if (typeVar == "int") { writing_arguments(nameVar, int_value, typeVar); }
                    if (typeVar == "short") { writing_arguments(nameVar, short_value, typeVar); }
                    if (typeVar == "char") { writing_arguments(nameVar, char_value, typeVar); }
                }
                else { throw exception(); };



            }
            else {
                nameVar = word;
                stream >> word;
                if (word == ")") {

                    Forbidden_symbols_and_name(nameVar);
                    endCommand = ")";
                    if (typeVar == "int") { writing_arguments(nameVar, int_value, typeVar); };
                    if (typeVar == "short") { writing_arguments(nameVar, short_value, typeVar); };
                    if (typeVar == "char") { writing_arguments(nameVar, char_value, typeVar); };
                }
                else { throw exception(); }
            }
        }
        else {
            auto semicolon = word.rfind(";");
            auto comma = word.rfind(",");
            auto equals = word.rfind("=");
            if ((semicolon != -1) && (comma == -1) && (equals == -1)) {
                if (word.length() == semicolon + 1) {
                    nameVar = word.erase(semicolon);
                    if ((nameVar.find(";") != -1) || (nameVar == "")) {
                        throw exception();
                    }
                    endCommand = ";";
                    Forbidden_symbols_and_name(nameVar);
                    if (typeVar == "int") { writing_arguments(nameVar, int_value, typeVar); }
                    if (typeVar == "short") { writing_arguments(nameVar, short_value, typeVar); }
                    if (typeVar == "char") { writing_arguments(nameVar, char_value, typeVar); }
                }
                else { throw exception(); };
            }
            else {
                if (comma != -1) {
                    if (word.length() == comma + 1) {
                        nameVar = word.erase(comma);
                        if ((nameVar.find(",") != -1) || (nameVar == "")) {
                            throw exception();
                        }
                        Forbidden_symbols_and_name(nameVar);
                        if (typeVar == "int") { writing_arguments(nameVar, int_value, typeVar); }
                        if (typeVar == "short") { writing_arguments(nameVar, short_value, typeVar); }
                        if (typeVar == "char") { writing_arguments(nameVar, char_value, typeVar); }
                        return ParsVars(stream, typeVar);

                    }
                    else { throw exception(); }
                }
                else {
                    nameVar = word;
                    stream >> word;
                    if (word == ";") {

                        Forbidden_symbols_and_name(nameVar);
                        endCommand = ";";
                        if (typeVar == "int") { writing_arguments(nameVar, int_value, typeVar); }
                        if (typeVar == "short") { writing_arguments(nameVar, short_value, typeVar); }
                        if (typeVar == "char") { writing_arguments(nameVar, char_value, typeVar); }
                    }
                    else {
                        if (word == "=") {
                            stream >> word;
                            semicolon = word.rfind(";");
                            if (semicolon != -1) {
                                if (word.length() == semicolon + 1) {
                                    word = word.erase(semicolon);
                                    if (typeVar == "char") {
                                        if ((word[0] == '\'') && (word[word.size() - 1] == '\'')) {
                                            word.erase(0, 1); word.erase(word.size() - 1);
                                        }
                                        else { throw exception(); }

                                    }
                                    endCommand = ";";
                                    if (typeVar == "int") { int_value = stoi(word); writing_arguments(nameVar, int_value, typeVar); };
                                    if (typeVar == "short") { int orowe = stoi(word); short_value = (short)orowe; writing_arguments(nameVar, short_value, typeVar); };
                                    if (typeVar == "char") { char_value = word[0]; writing_arguments(nameVar, char_value, typeVar); };
                                }
                                else { throw exception(); };
                            }
                            else {
                                comma = word.rfind(",");
                                if (comma != -1) {
                                    if (word.length() == comma + 1) {
                                        word = word.erase(comma);
                                        if (typeVar == "char") {
                                            if ((word[0] == '\'') && (word[word.size() - 1] == '\'')) {
                                                word.erase(0, 1); word.erase(word.size() - 1);
                                            }
                                            else { throw exception(); }
                                        }
                                     
                                        if (typeVar == "int") { int_value = stoi(word); writing_arguments(nameVar, int_value, typeVar); };
                                        if (typeVar == "short") { int orowe = stoi(word); short_value = (short)orowe; writing_arguments(nameVar, short_value, typeVar); };
                                        if (typeVar == "char") { char_value = word[0]; writing_arguments(nameVar, char_value, typeVar); };
                                        return ParsVars(stream, typeVar);
                                    }
                                    else { throw exception(); };

                                }
                                else {
                                    stream >> word;
                                    semicolon = word.rfind(";");
                                    if (word == ";") { endCommand = ";"; }
                                    else { throw exception(); }
                                }
                            }
                        }
                        else { throw exception(); };
                    };
                };
            };
        }
        if ((endCommand != ";") && (endCommand != ",") && (endCommand != ")"))
        {
            throw exception();
        }

    }
  

public:// в параметрах находится inFunc
    Parser(string file_with_code) : inFunc(true), curFor(0), curIf(0) {
        //open file_with_code
        //check is_file_opened
        //while(file opened)
        //call CheckConstruction()
        //while end
        //close file_with_code
       // WriteLogic();
    }          
    
};

int main()
{ //для проверки работы ParsVars
    string way_to_file = "Stream_of_data.txt";
    ifstream file;
    Parser parser(way_to_file);
    FuncElements funcselem;
    parser.funcs["Функция"] = funcselem;

    file.open(way_to_file);
    parser.ParsVars(file, parser.typeWar);
    
    cout << "   globalVarShort" << endl;
    for (auto command : parser.globalVarShort) {
        cout << command.first << "  " << command.second << endl;
    }
    cout << "    globalVarChar" << endl;
    for (auto command : parser.globalVarChar) {
        cout << command.first << "  " << command.second << endl;
    }
    cout << "   globalVarInt" << endl;
    for (auto command : parser.globalVarInt) {
        cout << command.first << "  " << command.second << endl;
    }
    cout << "   argInts" << endl;
    for (auto command : parser.funcs.at(parser.curFunc).argInts) {
        cout << command.first << "  " << command.second << endl;
    }
    cout << "   argChars" << endl;
    for (auto commanda : parser.funcs.at(parser.curFunc).argChars) {
        cout << commanda.first << "  " << commanda.second << endl;
    }
    cout << "   argShorts" << endl;
    for (auto commandf : parser.funcs.at(parser.curFunc).argShorts) {
        cout << commandf.first << "  " << commandf.second << endl;
    }
    cout << "   localInts" << endl;
    for (auto command : parser.funcs.at(parser.curFunc).localInts) {
        cout << command.first << "  " << command.second << endl;
    }
    cout << "   localChars" << endl;
    for (auto command : parser.funcs.at(parser.curFunc).localChars) {
        cout << command.first << "  " << command.second << endl;
    }
    cout << "   localShorts" << endl;
    for (auto command : parser.funcs.at(parser.curFunc).localShorts) {
        cout << command.first << "  " << command.second << endl;
    }
    cout << "Args" << endl;
    for (auto command : parser.funcs.at(parser.curFunc).dequeArgs) {
        cout  << "  " << command << endl;
    }
    cout << "Vars" << endl;
    for (auto command : parser.funcs.at(parser.curFunc).dequeVars) {
        cout<< "  " << command << endl;
    }
    cout << "LogicCommands"<<endl;
    for (auto command : parser.logicCommands) {
        printf_s("\n%s", command.c_str());

    }

}