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
};


class Parser {
public:
    int curFor; // для проверки, находится ли парсер в конструкции FOR и если да, то в какой вложенности конструкции
    int curIf; // для проверки, находится ли парсер в конструкции IF и если да, то в какой вложенности конструкции
    bool inFunc; // для проверки, находится ли парсер в функции (любой)
    string curFunc="Функция"; // если парсер в функции, то curFunc - название этой функции
    bool inArgs= 0;  // для проверки, находится ли парсер в параметрах функции 
    string typeWar = "char";//для проверки ParsVars

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
            this->logicCommands.push_back(nameVar + " = " + to_string(value));
        }
        
        
    }


    void writing_arguments_from_value(string nameVar,string valueVar, string typeVar, string place_valueVar){
        if (typeVar == "int") {
            if ((this->inFunc == 0) && (this->inArgs == 0)) {// записu global переменной в LogicCommands нету

                if (place_valueVar =="Global") { 
                    globalVarInt[nameVar] = globalVarInt[valueVar]; }
                if (place_valueVar == "Local"){
                    globalVarInt[nameVar]= funcs.at(curFunc).localInts[valueVar];
                }
                if (place_valueVar == "Arg") {
                    globalVarInt[nameVar] = funcs.at(curFunc).argInts[valueVar];;
                };
            }
            if ((this->inFunc == 0) && (this->inArgs == 1)) {// запись аргумента в LogicCommands нету
                if (place_valueVar == "Global") {
                    funcs.at(curFunc).argInts[nameVar] = globalVarInt[valueVar];
                }
                if (place_valueVar == "Local") {
                    funcs.at(curFunc).argInts[nameVar] = funcs.at(curFunc).localInts[valueVar];
                }
                if (place_valueVar == "Arg") {
                    funcs.at(curFunc).argInts[nameVar] = funcs.at(curFunc).argInts[valueVar];
                };
                funcs.at(curFunc).dequeArgs.push_back(typeVar);
            }
            if ((this->inFunc == 1) && (this->inArgs == 0)) {
                if (place_valueVar == "Global") {
                    funcs.at(curFunc).localInts[nameVar] = globalVarInt[valueVar];
                }
                if (place_valueVar == "Local") {
                    funcs.at(curFunc).localInts[nameVar] = funcs.at(curFunc).localInts[valueVar];
                }
                if (place_valueVar == "Arg") {
                    funcs.at(curFunc).localInts[nameVar] = funcs.at(curFunc).argInts[valueVar];
                };
                funcs.at(curFunc).dequeVars.push_back(typeVar);
                this->logicCommands.push_back(nameVar + " = " +valueVar);// запись локальной переменной в LogicCommands
            }

        };
        if (typeVar == "short") {
            if ((this->inFunc == 0) && (this->inArgs == 0)) {// записu global переменной в LogicCommands нету

                if (place_valueVar == "Global") {
                    globalVarShort[nameVar] = globalVarShort[valueVar];
                }
                if (place_valueVar == "Local") {
                    globalVarShort[nameVar] = funcs.at(curFunc).localShorts[valueVar];
                }
                if (place_valueVar == "Arg") {
                    globalVarShort[nameVar] = funcs.at(curFunc).argShorts[valueVar];;
                };
            }
            if ((this->inFunc == 0) && (this->inArgs == 1)) {// запись аргумента в LogicCommands нету
                if (place_valueVar == "Global") {
                    funcs.at(curFunc).argShorts[nameVar] = globalVarShort[valueVar];
                }
                if (place_valueVar == "Local") {
                    funcs.at(curFunc).argShorts[nameVar] = funcs.at(curFunc).localShorts[valueVar];
                }
                if (place_valueVar == "Arg") {
                    funcs.at(curFunc).argShorts[nameVar] = funcs.at(curFunc).argShorts[valueVar];
                };
                funcs.at(curFunc).dequeArgs.push_back(typeVar);
            }
            if ((this->inFunc == 1) && (this->inArgs == 0)) {
                if (place_valueVar == "Global") {
                    funcs.at(curFunc).localShorts[nameVar] = globalVarShort[valueVar];
                }
                if (place_valueVar == "Local") {
                    funcs.at(curFunc).localShorts[nameVar] = funcs.at(curFunc).localShorts[valueVar];
                }
                if (place_valueVar == "Arg") {
                    funcs.at(curFunc).localShorts[nameVar] = funcs.at(curFunc).argShorts[valueVar];
                };
                funcs.at(curFunc).dequeVars.push_back(typeVar);
                this->logicCommands.push_back(nameVar + " = " + valueVar);// запись локальной переменной в LogicCommands
            }

        };
        if (typeVar == "char") {
            if ((this->inFunc == 0) && (this->inArgs == 0)) {// записu global переменной в LogicCommands нету

                if (place_valueVar == "Global") {
                    globalVarChar[nameVar] = globalVarChar[valueVar];
                }
                if (place_valueVar == "Local") {
                    globalVarChar[nameVar] = funcs.at(curFunc).localChars[valueVar];
                }
                if (place_valueVar == "Arg") {
                    globalVarChar[nameVar] = funcs.at(curFunc).argChars[valueVar];;
                };
            }
            if ((this->inFunc == 0) && (this->inArgs == 1)) {// запись аргумента в LogicCommands нету
                if (place_valueVar == "Global") {
                    funcs.at(curFunc).argChars[nameVar] = globalVarChar[valueVar];
                }
                if (place_valueVar == "Local") {
                    funcs.at(curFunc).argChars[nameVar] = funcs.at(curFunc).localChars[valueVar];
                }
                if (place_valueVar == "Arg") {
                    funcs.at(curFunc).argChars[nameVar] = funcs.at(curFunc).argChars[valueVar];
                };
                funcs.at(curFunc).dequeArgs.push_back(typeVar);
            }
            if ((this->inFunc == 1) && (this->inArgs == 0)) {
                if (place_valueVar == "Global") {
                    funcs.at(curFunc).localChars[nameVar] = globalVarChar[valueVar];
                }
                if (place_valueVar == "Local") {
                    funcs.at(curFunc).localChars[nameVar] = funcs.at(curFunc).localChars[valueVar];
                }
                if (place_valueVar == "Arg") {
                    funcs.at(curFunc).localChars[nameVar] = funcs.at(curFunc).argChars[valueVar];
                };
                funcs.at(curFunc).dequeVars.push_back(typeVar);
                this->logicCommands.push_back(nameVar + " = " + valueVar);// запись локальной переменной в LogicCommands
            }

        };
    }






    string Check_Global_Vars(string word, string typeVar) {
        
        if (typeVar == "int") {
            auto it = this->globalVarInt.find(word);
            if (it != this->globalVarInt.end()) {
                return word;
            };
        };
        if (typeVar == "short") {
            auto is = this->globalVarShort.find(word);
            if (is != this->globalVarShort.end()) {
                return word;
            };
        };
        if (typeVar == "char") {
            auto ia = this->globalVarChar.find(word);
            if (ia != this->globalVarChar.end()) {
                return word;
            };
        };
        return "that_hell_this_arg_non_have_in_Global_Vars";

    };
    string Check_Local_Vars(string word, string typeVar) {

        if (typeVar == "int") {
            auto it = this->funcs.at(curFunc).localInts.find(word);
            if (it != this->funcs.at(curFunc).localInts.end()) {
             
                return word;
            };
        };
        if (typeVar == "short") {
            auto it = this->funcs.at(curFunc).localShorts.find(word);
            if (it != this->funcs.at(curFunc).localShorts.end()) {
               
                return word;
            };
        };
        if (typeVar == "char") {
            auto it = this->funcs.at(curFunc).localChars.find(word);
            if (it != this->funcs.at(curFunc).localChars.end()) {
                return word;
            };
        }
        return "that_hell_this_arg_non_have_in_Local_Vars!";

    };
    string Check_Arg_Vars(string word, string typeVar) {

        if (typeVar == "int") {
            auto it = this->funcs.at(curFunc).argInts.find(word);
            if (it != this->funcs.at(curFunc).argInts.end()) {

                return word;
            };
        };
        if (typeVar == "short") {
            auto it = this->funcs.at(curFunc).argShorts.find(word);
            if (it != this->funcs.at(curFunc).argShorts.end()) {
              return word;
            };
        };
        if (typeVar == "char") {
            auto it = this->funcs.at(curFunc).argChars.find(word);
            if (it != this->funcs.at(curFunc).argChars.end()) {
              return word;
            };
        };
        return "that_hell_this_arg_non_have_in_Arg_Vars!";

    };


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

                                    endCommand = ";";
                                    if (typeVar == "int") {
                                        if (Check_Global_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Global"); }
                                        else if(Check_Local_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Local"); }
                                        else if (Check_Arg_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Arg"); }
                                        else {
                                            int_value = stoi(word); writing_arguments(nameVar, int_value, typeVar);
                                        };
                                    };
                                    if (typeVar == "short") {
                                        if (Check_Global_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Global"); }
                                        else if (Check_Local_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Local"); }
                                        else if (Check_Arg_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Arg"); }
                                        else {
                                            int orowe = stoi(word); short_value = (short)orowe; writing_arguments(nameVar, short_value, typeVar);
                                        };
                                    };
                                    if (typeVar == "char") {
                                        if ((word[0] == '\'') && (word[word.size() - 1] == '\'')) {
                                            word.erase(0, 1); word.erase(word.size() - 1);
                                            char_value = word[0]; writing_arguments(nameVar, char_value, typeVar);
                                        }
                                        else {
                                            if (Check_Global_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Global"); }
                                            else if (Check_Local_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Local"); }
                                            else if (Check_Arg_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Arg"); }
                                            else { throw exception(); };
                                        };
                                    };
                                }
                                else { throw exception(); };
                            }
                            else {
                                comma = word.rfind(",");
                                if (comma != -1) {
                                    if (word.length() == comma + 1) {
                                        word = word.erase(comma);
                  
                                        if (typeVar == "int") {
                                            if (Check_Global_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Global"); }
                                            else if (Check_Local_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Local"); }
                                            else if (Check_Arg_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Arg"); }
                                            else {
                                                int_value = stoi(word); writing_arguments(nameVar, int_value, typeVar);
                                            }
                                        };
                                        if (typeVar == "short") {
                                            if (Check_Global_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Global"); }
                                            else if (Check_Local_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Local"); }
                                            else if (Check_Arg_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Arg"); }
                                            else {
                                                int orowe = stoi(word); short_value = (short)orowe; writing_arguments(nameVar, short_value, typeVar);
                                            }
                                        };
                                        if (typeVar == "char") {
                                            if ((word[0] == '\'') && (word[word.size() - 1] == '\'')) {
                                                word.erase(0, 1); word.erase(word.size() - 1);
                                                char_value = word[0]; writing_arguments(nameVar, char_value, typeVar);
                                            }
                                            else {
                                                if (Check_Global_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Global"); }
                                                else if (Check_Local_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Local"); }
                                                else if (Check_Arg_Vars(word, typeVar) == word) { writing_arguments_from_value(nameVar, word, typeVar, "Arg"); }
                                                else { throw exception(); };
                                            };
                                        };
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
    parser.globalVarInt["ftyu"] =666;
    parser.globalVarShort["fyu"] = 70;
    parser.globalVarChar["fu"] = 'r';
    parser.funcs.at(parser.curFunc).localInts["qwerty"] = 9;
    parser.funcs.at(parser.curFunc).localShorts["qwert"] = 5;
    parser.funcs.at(parser.curFunc).localChars["qwer"] = 'u';
    parser.funcs.at(parser.curFunc).argShorts["poiy"] = 31;
    parser.funcs.at(parser.curFunc).argChars["nyt"] ='z';
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