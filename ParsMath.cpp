#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <typeinfo>

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
public:
	int curFor; // для проверки, находится ли парсер в конструкции FOR и если да, то в какой вложенности конструкции
	int curIf; // для проверки, находится ли парсер в конструкции IF и если да, то в какой вложенности конструкции
	bool inFunc; // для проверки, находится ли парсер в функции (любой)
	bool inArgs; // для проверки, будут ли сейчас парситься аргументы функции
	bool isElse; // для проверки, является ли следующий код конструкцией else {...}
	string curFunc="Функция"; // если парсер в функции, то curFunc - название этой функции

    template <typename T>
    bool checkVar(ifstream& stream, const T& forParse, const string& current) {
        bool tmp = false;
        for (const auto& item : forParse) {
            if (item.first == current) {
                tmp = true;
                string checkOP;
                stream >> checkOP;
                if (checkOP == "=")
                    ParsMath(stream, current, item.first);
                else
                    throw exception();
            }
        }
        return tmp;
    }

	vector<string> logicCommands; // пропарсенная запись

	map<string, char> globalVarChar; // глоб. переменные char
	map<string, int> globalVarInt; // глоб. переменные int
	map<string, short> globalVarShort; // глоб. переменные short
	set<string> constStrings; // глоб. строки

	map<string, FuncElements> funcs; // имя функции + состав функции

    void Math_num(ifstream& stream, string sumb, const string& varToMath, string special_arg_for_logic) {
        string part;
        stream >> part;
        int num = 0;
        string first_value;
        string second_value;
        char operation;
        vector<string> value;
        if (sumb == "(") {
            while (part != ")") {

            }
        }
        if (sumb == "=") {
            Forbidden_symbols(part);
            value.push_back(part);
            stream >> part;
            operation = part[0];
            stream >> part;
            value.push_back(part);
            int i = 1;     //для отслеживания, перед операцией или после стоит переменная/число
            for (auto what_it : value) {
                if (i > 2) { throw exception(); };
                if (Check_Global_Vars(what_it, varToMath) == what_it) {
                    auto it = globalVarInt.find(what_it);
                    if (i = 1) { first_value = it->second; break; }
                    else { second_value = it->second; };
                }
                else if (Check_Local_Vars(what_it, varToMath) == what_it) {
                    auto it = this->funcs.at(curFunc).localInts.find(what_it);
                    if (i = 1) { first_value = it->second; break; }
                    else { second_value = it->second; };
                }
                else if (Check_Arg_Vars(what_it, varToMath) == what_it) {
                    auto it = this->funcs.at(curFunc).argInts.find(what_it);
                    if (i = 1) { first_value = it->second; break; }
                    else { second_value = it->second; };
                }
                else {
                    if (isdigit(what_it[0])) { first_value = stoi(what_it); };
                    i++;
                }
            }
            that_of_operation_is(first_value, second_value, operation, special_arg_for_logic);
        }
    };


  
    
	bool Skoba_check(ifstream& stream, const string& varToMath, string special_arg_for_logic){
        string part;
        while (part != ";") {
            stream >> part;
            if (part == "(") {
                Math_num(stream, "(", varToMath, special_arg_for_logic); return 1;
            }
            else if ((part == "\n")||(part == ")")) { throw exception(); };
        }
        return 0;
    }
    void that_of_operation_is (string first_value, string second_value,char sumb, string special_arg_for_logic) {
        if (sumb == '+') { addition(first_value, second_value,special_arg_for_logic); }
        else if (sumb == '*') { multiplication(first_value, second_value, special_arg_for_logic); }
        else if (sumb == '/') { division(first_value, second_value, special_arg_for_logic); }
        else { cout << sumb << " is not operation..."; throw exception(); };
    }
	string addition(string first_value, string second_value, string special_arg_for_logic){
        int oper= stoi(first_value) + stoi(second_value);
        
        return to_string(oper);
	}
    string multiplication(string first_value, string second_value, string special_arg_for_logic)
    {
        int oper = stoi(first_value) * stoi(second_value);
        return to_string(oper);
    }
    string division(string first_value, string second_value, string special_arg_for_logic) {
        {  if (stoi(first_value) == 0) { cout << "Вы ... на 0 делите..."; throw exception(); }
            int oper = stoi(first_value) / stoi(second_value);
            return to_string(oper);
        }
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

            globalVarChar[nameVar] = value;
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

            globalVarShort[nameVar] = value;
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

	void  Forbidden_symbols(string symbol) {

		vector<char> forbidden_symbols = { '!','@','"','#','№','$',';','^',':','&','?','*','<','>','`','~','/','|','\'','\\','.',',','+','-','%','=' };

		for (auto forbsimb : forbidden_symbols) {

			if (symbol.find(forbsimb) != -1) { throw exception(); };
		}
	}


	void ParsMath(ifstream& stream, const string& varToMath, string special_arg_for_logic) {
        string Special_arg_for_logic = special_arg_for_logic;
		string current, psevdo_current;
		vector<int> value;
		vector<char> operation;
		auto that_value = current;
        ifstream& stream1 = stream;
        if (Skoba_check(stream1, varToMath, special_arg_for_logic) == 0) {
            Math_num(stream1, "=", varToMath, special_arg_for_logic);
        }

	}



public:
	Parser(string file_with_code) : inArgs(false), inFunc(false), curFor(0), curIf(0) {
	}
}; //"class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char> >"



int main()
{
    string way_to_file = "Stream_of_data.txt";
    ifstream file;
    Parser parser(way_to_file);
    FuncElements funcselem;
    parser.funcs["Функция"] = funcselem;

    file.open(way_to_file);
    parser.globalVarInt["ftyu"] = 666;
    parser.globalVarShort["fyu"] = 70;
    parser.globalVarChar["fu"] = 'r';
    parser.funcs.at(parser.curFunc).localInts["qwerty"] = 9;
    parser.funcs.at(parser.curFunc).localShorts["qwert"] = 5;
    parser.funcs.at(parser.curFunc).localChars["qwer"] = 'u';
    parser.funcs.at(parser.curFunc).argShorts["poiy"] = 31;
    parser.ParsMath(file, "int","ftyu");

}