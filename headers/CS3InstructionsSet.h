#ifndef CS3INSTRUCTIONSSET_H
#define CS3INSTRUCTIONSSET_H
#include "headers/utilities.h"
#include "headers/functions.h"
#include "headers/translationfile.h"

#include <QString>
int add_operandes(int &addr, QByteArray &content, Instruction * instr, int size){
    int plVar15 = addr;
    int lVar5, lVar7;
    lVar7 = -1;
    do {
      instr->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
      lVar5 = lVar7 + size;
      lVar7 = lVar7 + 1;
    } while (content[plVar15+lVar5] != '\0');
    return lVar7;


}

void fun_140498b70(int &addr, QByteArray &content, Instruction * instr){
    //related to text/text formating
    QByteArray current_op_value;
    int addr_ = addr;
    bool start = false;
    bool start_text = false;
    int cnt = 0;
    do{
        unsigned char current_byte = content[addr];

        if (current_byte<0x20){
            if (current_byte == 0){
                current_op_value.clear();
                current_op_value[0] = 0;
                instr->AddOperande(operande(addr,"byte", current_op_value));
                addr++;
                return;
            }
            else if (current_byte == 0x10){
                current_op_value.clear();
                current_op_value.push_back(current_byte);
                instr->AddOperande(operande(addr,"byte", current_op_value));
                current_op_value.clear();
                addr++;
                instr->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            }
            else if ((current_byte > 0x10)&&(current_byte < 0x13)){
                current_op_value.clear();
                current_op_value.push_back(current_byte);
                instr->AddOperande(operande(addr,"byte", current_op_value));
                current_op_value.clear();
                addr++;
                instr->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            }
            else if (current_byte == 0x2){
                qDebug() << " done text";
                start_text = false;
                instr->AddOperande(operande(addr_,"string", current_op_value));
                current_op_value.clear();
                current_op_value.push_back(current_byte);
                instr->AddOperande(operande(addr,"byte", current_op_value));
                current_op_value.clear();
                addr++;
            }
            else if ((start_text)&&current_byte == 0x1){
                current_op_value.push_back(current_byte);
                addr++;
            }
            else{
                current_op_value.clear();
                current_op_value.push_back(current_byte);
                instr->AddOperande(operande(addr,"byte", current_op_value));
                addr++;
                current_op_value.clear();
            }

        }
        else{

                if (current_byte == 0x23){

                    current_op_value.push_back(0x23);
                    addr_ = addr;//beginning of some format stuff
                    addr++;
                    current_byte = content[addr];
                    while (current_byte!=0){


                        if ((((current_byte == 0x45) || (current_byte == 0x65)) || (current_byte == 0x4d)) ||
                           (current_byte == 0x42)||(current_byte == 0x48)||(current_byte == 0x56)||(current_byte == 0x4b) ||
                            (current_byte == 0x6b) || (current_byte == 0x46)) {
                            current_op_value.push_back(current_byte);
                            addr++;
                            current_byte = content[addr];

                              if (current_byte != 0x5f) {
                                  if (current_byte == 0x5b) {
                                      do{
                                          current_op_value.push_back(current_byte);
                                          addr++;
                                          current_byte = content[addr];
                                      }
                                      while(current_byte!=0x5D);
                                      current_op_value.push_back(current_byte);
                                      addr++;
                                      current_byte = content[addr];
                                  }
                                  break;
                              }
                              else{
                                  current_op_value.push_back(current_byte);
                                  addr++;
                                  current_byte = content[addr];
                                  current_op_value.push_back(current_byte);
                                  addr++;
                                  current_byte = content[addr];
                                  break;


                              }

                           }
                        else if(((current_byte + 0xb7 & 0xdf) == 0)||(current_byte == 0x50)||(current_byte == 0x54)||(current_byte == 0x57)||
                                 (current_byte == 0x53)||(current_byte == 0x73)||(current_byte == 0x43)||(current_byte == 99)||(current_byte == 0x78)||
                                 (current_byte == 0x79)||(current_byte == 0x47)||(current_byte == 0x44)) {
                                 current_op_value.push_back(current_byte);
                                 addr++;
                                 current_byte = content[addr];
                                 break;

                        }
                        else{
                            qDebug() << hex << (int) current_byte << "  " << hex << addr;
                            current_op_value.push_back(current_byte);
                            addr++;
                            current_byte = content[addr];
                        }


                    }
                }
                else{
                    //here, should be actual text (I think)
                    if (!start_text) {
                        qDebug() << " starting text!";
                        instr->AddOperande(operande(addr_,"string", current_op_value));
                        current_op_value.clear();
                        start_text = true;
                        addr_ = addr;
                    }
                    current_op_value.push_back(current_byte);
                    addr++;

                }
            }

        cnt++;
    }
    while(cnt<9999);

}
void fun_1403c90e0(int &addr, QByteArray &content, Instruction * instr, int param){
    QByteArray control_byte3_arr = ReadSubByteArray(content, addr, 1);
    instr->AddOperande(operande(addr,"byte", control_byte3_arr));
    unsigned char control_byte3 = (unsigned char)control_byte3_arr[0];

    if (control_byte3 == '\x11'){
        instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
        instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

    }
    else if (control_byte3 != '3'){

        if (control_byte3 == '\"'){
            instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

        }
        else if (control_byte3 != 0x44){

            if (control_byte3 == 0xDD){

                //there is a XOR,XOR EDI which causes a jump that ignores the strcpy
                if (param!=0)instr->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                instr->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            }
            else if (control_byte3 == 0xFF){
                instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                instr->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            }
            else{
                if (control_byte3 != 0xEE){

                }
                else{
                    instr->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    instr->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                }
            }


        }

    }
}
void sub05(int &addr, QByteArray &content, Instruction * instr){
    QByteArray control_byte = ReadSubByteArray(content, addr, 1);
    instr->AddOperande(operande(addr,"byte", control_byte));

    while ((int)control_byte[0]!=1){

            switch ((unsigned char)control_byte[0])  {
                case 0x0:
                    instr->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    break;

                case 0x1c:{
                    //the next byte is the OP code for a new instruction
                    QByteArray OPCode_byte = ReadSubByteArray(content, addr, 1);
                    instr->AddOperande(operande(addr,"byte", OPCode_byte));
                    std::shared_ptr<Instruction> instr2 = instr->Maker->CreateInstructionFromDAT(addr, content,0);
                    instr->AddOperande(operande(addr,"bytearray", instr2->getBytes()));
                    break;
                }
                case 0x1e:
                    instr->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x1f:
                case 0x20:
                case 0x23:

                    instr->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x21:
                    instr->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                default:
                    ;
            }

            control_byte = ReadSubByteArray(content, addr, 1);
            instr->AddOperande(operande(addr,"byte", control_byte));

        }
}
class CreateMonsters : public Instruction
{
    public:
    CreateMonsters():Instruction(-1,256,nullptr){}
    CreateMonsters(int addr, Builder *Maker):Instruction(addr,"CreateMonsters", 256, Maker){}
    CreateMonsters(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"CreateMonsters", 256,Maker){
        int initial_addr = addr;
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));

        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));

        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2))); //RSI + 0x28
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4))); //0x2C

        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//0x30
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//0x32
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//0x34
        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//0x36

        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//0x38

        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//0x3C
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//0x40
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//0x44
        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//0x48
        //0x4C The first part is done
        //from here, it's just a guess. There is a maximum of 8 enemies hardcoded in the function.

        QByteArray array = ReadSubByteArray(content, addr, 4);
        this->AddOperande(operande(addr,"int", array));
        int first = ReadIntegerFromByteArray(0,array);
        do{
            int counter = 0;

            do{
                counter++;
                QByteArray monsters_name = ReadStringSubByteArray(content, addr);
                this->AddOperande(operande(addr,"string", monsters_name));

                QByteArray remaining = ReadSubByteArray(content, addr, 16-monsters_name.size()-1);
                this->AddOperande(operande(addr,"bytearray", remaining));
            }
            while(counter < 0x8);
            for (int idx_byte = 0; idx_byte < 8; idx_byte++) this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

            this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr, 8))); //??

            QByteArray array = ReadSubByteArray(content, addr, 4);
            this->AddOperande(operande(addr,"int", array));
            first = ReadIntegerFromByteArray(0,array);

        }
        while(first != -1);

        this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr, 0x18))); //??

    }


};
class EffectsInstr : public Instruction
{
    public:
    EffectsInstr():Instruction(-1,257,nullptr){}
    EffectsInstr(int addr, Builder *Maker):Instruction(addr,"EffectsInstr", 257, Maker){}
    EffectsInstr(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"EffectsInstr", 257,Maker){
        unsigned char current_byte = content[addr];
        while (current_byte!=0x01){

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            QByteArray str = ReadStringSubByteArray(content, addr);
            this->AddOperande(operande(addr,"string", str));
            QByteArray remaining = ReadSubByteArray(content, addr, 0x28-8-str.size()-1);
            this->AddOperande(operande(addr,"bytearray", remaining));

            current_byte = content[addr];
        }
        qDebug() << "done";
    }


};
class OPCode0 : public Instruction
{
    public:
    OPCode0():Instruction(-1,0,nullptr){}
    OPCode0(int addr, Builder *Maker):Instruction(addr,"Instruction 0", 0, Maker){}
    OPCode0(int &addr, QByteArray &content,Builder *Maker):Instruction(addr,"Instruction 0", 0,Maker){
        addr++;

        QByteArray control_byte = ReadSubByteArray(content, addr, 1);
        this->AddOperande(operande(addr,"byte", control_byte));
    }


};
class OPCode1 : public Instruction
{
    public:
    OPCode1():Instruction(-1,1,nullptr){}
    OPCode1(int addr, Builder *Maker):Instruction(addr,"Return",1,Maker){}
    OPCode1(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"Return", 1,Maker){

    }


};
class OPCode2 : public Instruction
{
    public:
    OPCode2():Instruction(-1,2,nullptr){}
    OPCode2(int addr, Builder *Maker):Instruction(addr,"Call",2,Maker){}
    OPCode2(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"Call", 2,Maker){
            addr++;

            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            QByteArray function_name = ReadStringSubByteArray(content, addr);
            this->AddOperande(operande(addr,"string", function_name));
            QByteArray control_byte2_arr = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte2_arr));

            int control_byte2 = (int)control_byte2_arr[0];

            if (control_byte2!=0){
                for (int i = 0; i < control_byte2; i++){
                    fun_1403c90e0(addr, content, this, 0);
                }

            }
            switch((unsigned char)control_byte[0]){
                case 0xB:

                    qDebug() << "Calling function " << ConvertBytesToString(function_name);
                    //here we're calling a function that was defined at the beginning
                    std::vector<function>::iterator it = find_function_by_name(Maker->FunctionsToParse,ConvertBytesToString(function_name));

                    if (!std::count(Maker->FunctionsParsed.begin(), Maker->FunctionsParsed.end(), *it)){
                        Maker->ReadIndividualFunction(*it,content);
                        Maker->FunctionsParsed.push_back(*it);
                    }
                    //Maker->FunctionsToParse.erase(it);
                    break;
            }



    }


};
class OPCode3 : public Instruction
{
    public:
    OPCode3():Instruction(-1,3,nullptr){}
    OPCode3(int addr, Builder *Maker):Instruction(addr,"???",3,Maker){}
    OPCode3(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 3,Maker){
        addr++;
        this->AddOperande(operande(addr,"pointer", ReadSubByteArray(content, addr, 4)));
    }


};
class OPCode4 : public Instruction
{
    public:
    OPCode4():Instruction(-1,4,nullptr){}
    OPCode4(int addr, Builder *Maker):Instruction(addr,"???",4,Maker){}
    OPCode4(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 4,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"pointer", ReadSubByteArray(content, addr, 4)));
    }
};
class OPCode5 : public Instruction
{
    public:
    OPCode5():Instruction(-1,5,nullptr){}
    OPCode5(int addr, Builder *Maker):Instruction(addr,"???",5,Maker){}
    OPCode5(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 5,Maker){
            addr++;
            sub05(addr, content, this);
            this->AddOperande(operande(addr,"pointer", ReadSubByteArray(content, addr, 4))); // pointer
    }
};
class OPCode7 : public Instruction
{
    public:
    OPCode7():Instruction(-1,0x7,nullptr){}
    OPCode7(int addr, Builder *Maker):Instruction(addr,"???",0x7,Maker){}
    OPCode7(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x7,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            switch ((unsigned char)control_byte[0])  {
                case 0:
                    fun_1403c90e0(addr, content, this, 0);

                    break;
                case 1:
                    fun_1403c90e0(addr, content, this, 0);
                    break;
                case 2:
                    fun_1403c90e0(addr, content, this, 0);
                    break;
            }
    }


};
class OPCode8 : public Instruction
{
    public:
    OPCode8():Instruction(-1,8,nullptr){}
    OPCode8(int addr, Builder *Maker):Instruction(addr,"???",8,Maker){}
    OPCode8(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 8,Maker){
            addr++;
            sub05(addr, content, this);

    }
};
class OPCodeC : public Instruction
{
    public:
    OPCodeC():Instruction(-1,0xC,nullptr){}
    OPCodeC(int addr, Builder *Maker):Instruction(addr,"???",0xC,Maker){}
    OPCodeC(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0xC,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

    }
};
class OPCode10 : public Instruction
{
    public:
    OPCode10():Instruction(-1,0x10,nullptr){}
    OPCode10(int addr, Builder *Maker):Instruction(addr,"???",0x10,Maker){}
    OPCode10(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x10,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

    }
};
class OPCode11 : public Instruction
{
    public:
    OPCode11():Instruction(-1,0x11,nullptr){}
    OPCode11(int addr, Builder *Maker):Instruction(addr,"???",0x11,Maker){}
    OPCode11(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x11,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

    }
};
class OPCode13 : public Instruction
{
    public:
    OPCode13():Instruction(-1,0x13,nullptr){}
    OPCode13(int addr, Builder *Maker):Instruction(addr,"???",0x13,Maker){}
    OPCode13(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x13,Maker){
            addr++;
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

    }
};
class OPCode14 : public Instruction
{
    public:
    OPCode14():Instruction(-1,0x14,nullptr){}
    OPCode14(int addr, Builder *Maker):Instruction(addr,"???",0x14,Maker){}
    OPCode14(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x14,Maker){
            addr++;
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

    }
};
class OPCode16 : public Instruction
{
    public:
    OPCode16():Instruction(-1,0x16,nullptr){}
    OPCode16(int addr, Builder *Maker):Instruction(addr,"???",0x16,Maker){}
    OPCode16(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x16,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

    }
};
class OPCode18 : public Instruction
{
    public:
    OPCode18():Instruction(-1,0x18,nullptr){}
    OPCode18(int addr, Builder *Maker):Instruction(addr,"???",0x18,Maker){}
    OPCode18(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x18,Maker){
            addr++;
            sub05(addr, content, this);

    }
};
class OPCode1D : public Instruction
{
    public:
    OPCode1D():Instruction(-1,0x1D,nullptr){}
    OPCode1D(int addr, Builder *Maker):Instruction(addr,"???",0x1D,Maker){}
    OPCode1D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x1D,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//6
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//10
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//E
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//0x12
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//0x16
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//0x1A
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//0x1E
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//0x22
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            QByteArray IDFun_bytearray = ReadSubByteArray(content, addr,4);
            int IDfun = ReadIntegerFromByteArray(0, IDFun_bytearray);
            if (IDfun!=-1){
                std::vector<function>::iterator it = find_function_by_ID(Maker->FunctionsToParse,IDfun);
                if (!std::count(Maker->FunctionsParsed.begin(), Maker->FunctionsParsed.end(), *it)){
                    qDebug() << "Calling CreateMonster function at addr " << hex << it->actual_addr << " ID: "<< hex << it->ID;
                    it->AddInstruction(std::make_shared<CreateMonsters>(it->actual_addr,content,Maker));
                    Maker->FunctionsParsed.push_back(*it);
                }
            }
            this->AddOperande(operande(addr,"int", IDFun_bytearray));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

    }
};
class OPCode1E : public Instruction
{
    public:
    OPCode1E():Instruction(-1,0x1E,nullptr){}
    OPCode1E(int addr, Builder *Maker):Instruction(addr,"???",0x1E,Maker){}
    OPCode1E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x1E,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

    }
};
class OPCode1F : public Instruction
{
    public:
    OPCode1F():Instruction(-1,0x1F,nullptr){}
    OPCode1F(int addr, Builder *Maker):Instruction(addr,"???",0x1F,Maker){}
    OPCode1F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x1F,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));


    }
};
class OPCode20 : public Instruction
{
    public:
    OPCode20():Instruction(-1,0x20,nullptr){}
    OPCode20(int addr, Builder *Maker):Instruction(addr,"???",0x20,Maker){}
    OPCode20(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x20,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            fun_1403c90e0(addr, content, this, 0);
            fun_1403c90e0(addr, content, this, 0);
            fun_1403c90e0(addr, content, this, 0);
    }


};
class OPCode21 : public Instruction
{
    public:
    OPCode21():Instruction(-1,0x21,nullptr){}
    OPCode21(int addr, Builder *Maker):Instruction(addr,"???",0x21,Maker){}
    OPCode21(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x21,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
    }


};
class OPCode22 : public Instruction
{
    public:
    OPCode22():Instruction(-1,0x22,nullptr){}
    OPCode22(int addr, Builder *Maker):Instruction(addr,"???",0x22,Maker){}
    OPCode22(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x22,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            fun_140498b70(addr, content, this);
    }


};
class OPCode23 : public Instruction
{
    public:
    OPCode23():Instruction(-1,0x23,nullptr){}
    OPCode23(int addr, Builder *Maker):Instruction(addr,"???",0x23,Maker){}
    OPCode23(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x23,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            switch ((unsigned char)control_byte[0])  {
                case 0:
                case 5:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 1:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 2:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
            }
    }


};
class OPCode24 : public Instruction
{
    public:
    OPCode24():Instruction(-1,0x24,nullptr){}
    OPCode24(int addr, Builder *Maker):Instruction(addr,"???",0x24,Maker){}
    OPCode24(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x24,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            fun_140498b70(addr, content, this);

    }


};
class OPCode25 : public Instruction
{
    public:
    OPCode25():Instruction(-1,0x25,nullptr){}
    OPCode25(int addr, Builder *Maker):Instruction(addr,"???",0x25,Maker){}
    OPCode25(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x25,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
    }


};
class OPCode26 : public Instruction
{
    public:
    OPCode26():Instruction(-1,0x26,nullptr){}
    OPCode26(int addr, Builder *Maker):Instruction(addr,"???",0x26,Maker){}
    OPCode26(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x26,Maker){
            addr++;
    }


};
class OPCode27 : public Instruction
{
    public:
    OPCode27():Instruction(-1,0x27,nullptr){}
    OPCode27(int addr, Builder *Maker):Instruction(addr,"???",0x27,Maker){}
    OPCode27(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x27,Maker){
            addr++;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
    }


};
class OPCode2B : public Instruction
{
    public:
    OPCode2B():Instruction(-1,0x2B,nullptr){}
    OPCode2B(int addr, Builder *Maker):Instruction(addr,"???",0x2B,Maker){}
    OPCode2B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x2B,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

            if ((unsigned char)control_byte[0] == 0){
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            }
            if ((unsigned char)control_byte[0] == 1){
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            }


    }


};
class OPCode2F : public Instruction
{
    public:
    OPCode2F():Instruction(-1,0x2F,nullptr){}
    OPCode2F(int addr, Builder *Maker):Instruction(addr,"???",0x2F,Maker){}
    OPCode2F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x2F,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            switch ((unsigned char)control_byte[0])  {
            case 1:
            case 0:
            case 2:
            case 4:
            case 5:
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            case 6:
            case 7:
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                break;
            case 8:

                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));//16
                break;
            case 9:
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                break;
            case 0xA:
            case 0xB:
            case 0xC:
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            case 0xE:
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            case 0xF:
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                break;
            }

    }


};

class OPCode32 : public Instruction
{
    public:
    OPCode32():Instruction(-1,0x32,nullptr){}
    OPCode32(int addr, Builder *Maker):Instruction(addr,"???",0x32,Maker){}
    OPCode32(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x32,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            switch ((unsigned char)control_byte[0])  {
            case '\n':
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                return;


            case '\v':
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                return;

            case '\f':
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                fun_1403c90e0(addr,content,this, 1);
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                fun_1403c90e0(addr,content,this, 0);
                fun_1403c90e0(addr,content,this, 0);
                fun_1403c90e0(addr,content,this, 0);
                fun_1403c90e0(addr,content,this, 0);//0xFF
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                fun_1403c90e0(addr,content,this, 0);
                fun_1403c90e0(addr,content,this, 0);
                fun_1403c90e0(addr,content,this, 0);
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                return;
            }
            if (((unsigned char)control_byte[0] - 0xD) < 2) {
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            }
            else if (1 < (control_byte[0] - 0xfU)){
                switch((unsigned char)control_byte[0]){
                case '\x11':
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case '\x12':

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                case '\x13':
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                     break;

                default:
                    if ((unsigned char)(control_byte[0] - 0x14U) < 2){
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//3
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//F
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    }
                    break;

                }

            }
            else{
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

            }
            switch((unsigned char)control_byte[0]){
                case '\x16':
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                case '\x17':
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                case '\x18':
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                case '\x19':

                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                case '\x1A':
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                case '\x1B':
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                case '\x1C':
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                    break;
                case '\x1D':
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    break;
                }

          }



};
class OPCode35 : public Instruction
{
    public:
    OPCode35():Instruction(-1,0x35,nullptr){}
    OPCode35(int addr, Builder *Maker):Instruction(addr,"???",0x35,Maker){}
    OPCode35(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x35,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

    }
};
class OPCode36 : public Instruction
{
    public:
    OPCode36():Instruction(-1,0x36,nullptr){}
    OPCode36(int addr, Builder *Maker):Instruction(addr,"???",0x36,Maker){}
    OPCode36(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x36,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch ((unsigned char)control_byte[0])  {
                case 0:
                    break;
                case 2:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 3:
                case 0x14:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x4:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                case 0x5:
                case 0x16:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case 0x6:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case 0x7:
                case 0x17:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case 0x8:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case 0x9:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                case 0xA:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//0
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//4
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//8->c
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//c->E
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//E->10
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//10->12
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//12->14
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//14->16
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                case 0xB:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case 0xC:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case 0xD:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case 0xE:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case 0xF:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                    break;
                case 0x11:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                case 0x12:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x13:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    int lVar7 = add_operandes(addr, content, this, 3);
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;}
                case 0x15:
                case 0x1f:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;}
                case 0x19:
                case 0x18:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;}
                case 0x1A:{
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//2->6
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//6->10
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//10->14 (d)
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//(e)
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//(f)
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));//(0x10)
                    break;}
                case 0x1B:{

                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                    break;}
                case 0x28:
                case 0x29:{

                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                    break;}
                case 0x2A:
                case 0x2B:{
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;}
                case 0x2C:
                case 0x2D:{
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;}
                case 0x2E:{
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;}
            }
    }
};
class OPCode37 : public Instruction
{
    public:
    OPCode37():Instruction(-1,0x37,nullptr){}
    OPCode37(int addr, Builder *Maker):Instruction(addr,"???",0x37,Maker){}
    OPCode37(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x37,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
    }
};
class OPCode38 : public Instruction
{
    public:
    OPCode38():Instruction(-1,0x38,nullptr){}
    OPCode38(int addr, Builder *Maker):Instruction(addr,"???",0x38,Maker){}
    OPCode38(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x38,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
    }
};

class OPCode39 : public Instruction
{
    public:
    OPCode39():Instruction(-1,0x39,nullptr){}
    OPCode39(int addr, Builder *Maker):Instruction(addr,"???",0x39,Maker){}
    OPCode39(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x39,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
    }
};

class OPCode3A : public Instruction
{
    public:
    OPCode3A():Instruction(-1,0x3A,nullptr){}
    OPCode3A(int addr, Builder *Maker):Instruction(addr,"???",0x3A,Maker){}
    OPCode3A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x3A,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch ((unsigned char)control_byte[0])  {
                case 0x0:
                case 0x4:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x1:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x2:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x3:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x5:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x6:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case 0x7:

                    break;
                case 0x8:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x9:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
            }


    }
};
class OPCode3B : public Instruction
{
    public:
    OPCode3B():Instruction(-1,0x3B,nullptr){}
    OPCode3B(int addr, Builder *Maker):Instruction(addr,"???",0x3B,Maker){}
    OPCode3B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x3B,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            switch ((unsigned char)control_byte[0])  {
                case 0xFB:
                case 0xFD:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0xFE:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0xFF:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0:
                case 0x32:
                    fun_1403c90e0(addr, content, this, 1);
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    fun_1403c90e0(addr, content, this, 1);

                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//0->4
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//4->8
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//8->A
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//A->C
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//C->10
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//10->12
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//12->14
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//14->18
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//18->1C

                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));//x

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//1C+x
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//1E+x
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//20+x
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//22+x
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//24+x
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//26+x
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//28+x
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));//2A+x
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//2C+x

                    break;
                case 1:
                case 0x33:
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    break;
                case 2:
                case 0x34:
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    break;
                case 3:
                case 0x35:
                    fun_1403c90e0(addr, content, this, 1);
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 4:
                case 0x36:
                    fun_1403c90e0(addr, content, this, 1);
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 5:
                case 0x37://very weird one; caution

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

                    break;
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 0xb:
                case 0xc:
                case 0xd:
                case 0xe:
                case 0xf:
                case 0x10:
                case 0x11:
                case 0x12:
                case 0x13:
                case 0x14:
                case 0x15:
                case 0x16:
                case 0x17:
                case 0x18:
                case 0x19:
                case 0x1a:
                case 0x1b:
                case 0x1c:
                case 0x1d:
                case 0x1e:
                case 0x1f:
                case 0x20:
                case 0x21:
                case 0x22:
                case 0x23:
                case 0x24:
                case 0x25:
                case 0x26:
                case 0x27:
                case 0x28:
                case 0x29:
                case 0x2a:
                case 0x2b:
                case 0x2c:
                case 0x2d:
                case 0x2e:
                case 0x2f:
                case 0x30:
                case 0x31:
                case 0x38:
                case 0x3f:
                case 0x40:
                case 0x41:
                case 0x42:
                case 0x43:
                case 0x44:
                case 0x45:
                case 0x46:
                case 0x47:
                case 0x48:
                case 0x49:
                case 0x4a:
                case 0x4b:
                case 0x4c:
                case 0x4d:
                case 0x4e:
                case 0x4f:
                case 0x50:
                case 0x51:
                case 0x52:
                case 0x53:
                case 0x54:
                case 0x55:
                case 0x56:
                case 0x57:
                case 0x58:
                case 0x59:
                case 0x5a:
                case 0x5b:
                case 0x5c:
                case 0x5d:
                case 0x5e:
                case 0x5f:
                case 0x60:
                case 0x61:
                case 0x62:
                case 99:
                case 0x66:
                case 0x67:
                case 0x68:
                case 0x69:
                case 0x6a:
                case 0x6b:
                case 0x6c:
                case 0x6d:
                case 0x6e:
                case 0x6f:
                case 0x70:
                case 0x71:
                case 0x72:
                case 0x73:
                case 0x74:
                case 0x75:
                case 0x76:
                case 0x77:
                case 0x78:
                case 0x79:
                case 0x7a:
                case 0x7b:
                case 0x7c:
                case 0x7d:
                case 0x7e:
                case 0x7f:
                case 0x80:
                case 0x81:
                case 0x82:
                case 0x83:
                case 0x84:
                case 0x85:
                case 0x86:
                case 0x87:
                case 0x88:
                case 0x89:
                case 0x8a:
                case 0x8b:
                case 0x8c:
                case 0x8d:
                case 0x8e:
                case 0x8f:
                case 0x90:
                case 0x91:
                case 0x92:
                case 0x93:
                case 0x94:
                case 0x95:
                case 0x3D:
                  break;
                case 0x39:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x3A:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    fun_1403c90e0(addr, content, this, 1);
                    break;
                case 0x3B:
                case 0x3C:
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x3E:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 100:
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x65:
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//0->4
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//4->8
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//8->C
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//C->10
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//10->14
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//14->18
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//18->1C
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//1C->20
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//20->24
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//24->28
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//28->2C
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//2C->30
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//30->34
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//34->38
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//38->3C
                    break;
                case 0x96:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                default:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
            }


    }
};
class OPCode3C : public Instruction
{
    public:
    OPCode3C():Instruction(-1,0x3C,nullptr){}
    OPCode3C(int addr, Builder *Maker):Instruction(addr,"???",0x3C,Maker){}
    OPCode3C(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x3C,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            switch ((unsigned char)control_byte[0])  {
                case 0x1:
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
                case 0x3:
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0x4:
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;

            }


    }
};
class OPCode3D : public Instruction
{
    public:
    OPCode3D():Instruction(-1,0x3D,nullptr){}
    OPCode3D(int addr, Builder *Maker):Instruction(addr,"???",0x3D,Maker){}
    OPCode3D(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x3D,Maker){
            addr++;

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

    }
};
class OPCode3E : public Instruction
{
    public:
    OPCode3E():Instruction(-1,0x3E,nullptr){}
    OPCode3E(int addr, Builder *Maker):Instruction(addr,"???",0x3E,Maker){}
    OPCode3E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x3E,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
    }
};
class OPCode3F : public Instruction
{
    public:
    OPCode3F():Instruction(-1,0x3F,nullptr){}
    OPCode3F(int addr, Builder *Maker):Instruction(addr,"???",0x3F,Maker){}
    OPCode3F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x3F,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
    }
};
class OPCode40 : public Instruction
{
    public:
    OPCode40():Instruction(-1,0x40,nullptr){}
    OPCode40(int addr, Builder *Maker):Instruction(addr,"???",0x40,Maker){}
    OPCode40(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x40,Maker){
            addr++;

            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//1
            QByteArray control_short = ReadSubByteArray(content, addr, 2);
            short control = ReadShortFromByteArray(0, control_short);
            this->AddOperande(operande(addr,"short", control_short));//3
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//5
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//9
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//D

            if ((ushort)(control + 0x1feU) < 3){

                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//11 -> 15
            }
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            if (control == -0x1fb) {//I'm not sure at all of this behavior, but at the same time I'm not even sure it happens anywhere (lazy)
              while (content[addr] != '\0'){
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

              }
            }

    }
};
class OPCode41 : public Instruction
{
    public:
    OPCode41():Instruction(-1,0x41,nullptr){}
    OPCode41(int addr, Builder *Maker):Instruction(addr,"0x41",0x41,Maker){}
    OPCode41(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x41", 0x41,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

    }
};
class OPCode42 : public Instruction
{
    public:
    OPCode42():Instruction(-1,0x42,nullptr){}
    OPCode42(int addr, Builder *Maker):Instruction(addr,"???",0x42,Maker){}
    OPCode42(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x42,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));//1
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//2
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//4
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//8
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//C
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//10
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//14
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//18
            qFatal("vérifier 42!!");

    }
};
class OPCode43 : public Instruction
{
    public:
    OPCode43():Instruction(-1,0x43,nullptr){}
    OPCode43(int addr, Builder *Maker):Instruction(addr,"???",0x43,Maker){}
    OPCode43(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x43,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

            switch ((unsigned char)control_byte[0])  {
                case 0xFF:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case 0xFE:
                case 0x05:
                case 0x69:
                case 0x06:
                case 0x6A:
                case 0x10:
                case 0x0B:
                case 0x0C:

                    break;
                default:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;

            }

    }
};
class OPCode44 : public Instruction
{
    public:
    OPCode44():Instruction(-1,0x44,nullptr){}
    OPCode44(int addr, Builder *Maker):Instruction(addr,"0x44",0x44,Maker){}
    OPCode44(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x44", 0x44,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
    }
};
class OPCode45 : public Instruction
{
    public:
    OPCode45():Instruction(-1,0x45,nullptr){}
    OPCode45(int addr, Builder *Maker):Instruction(addr,"0x45",0x45,Maker){}
    OPCode45(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x45", 0x45,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
    }
};
class OPCode46 : public Instruction
{
    public:
    OPCode46():Instruction(-1,0x46,nullptr){}
    OPCode46(int addr, Builder *Maker):Instruction(addr,"0x46",0x46,Maker){}
    OPCode46(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x46", 0x46,Maker){

            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            switch ((unsigned char)control_byte[0])  {
                case 3:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                case 1:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
            }
    }
};

class OPCode47 : public Instruction
{
    public:
    OPCode47():Instruction(-1,0x47,nullptr){}
    OPCode47(int addr, Builder *Maker):Instruction(addr,"???",0x47,Maker){}
    OPCode47(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x47,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
    }
};
class OPCode48 : public Instruction
{
    public:
    OPCode48():Instruction(-1,0x48,nullptr){}
    OPCode48(int addr, Builder *Maker):Instruction(addr,"???",0x48,Maker){}
    OPCode48(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x48,Maker){
            addr++;
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
    }
};
class OPCode49 : public Instruction
{
    public:
    OPCode49():Instruction(-1,0x49,nullptr){}
    OPCode49(int addr, Builder *Maker):Instruction(addr,"0x49",0x49,Maker){}
    OPCode49(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x49", 0x49,Maker){

            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));


            switch ((unsigned char)control_byte[0])  {
                case 0:
                case 1:
                case 4:
                case 5:
                case 6:
                case 0x11:
                case '\t':
                case '\n':
                case '\v':
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case '\b':
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                case '\f':
                case 0x17:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case '\r': // pas sûr
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr,0x18)));
                    break;
                case '\x0e':
                case '\x0f':
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;

            }
    }
};
class OPCode4B : public Instruction
{
    public:
    OPCode4B():Instruction(-1,0x4B,nullptr){}
    OPCode4B(int addr, Builder *Maker):Instruction(addr,"0x4B",0x4B,Maker){}
    OPCode4B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x4B", 0x4B,Maker){

            addr++;
            QByteArray control_short = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr,"short", control_short));
            short control_shrt = ReadShortFromByteArray(0, control_short);
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            /*switch (control_shrt)  {
                case 0xfff8:

                    break;
                case 0xfff9:

                    break;

            }*/
    }
};
class OPCode4E : public Instruction
{
    public:
    OPCode4E():Instruction(-1,0x4E,nullptr){}
    OPCode4E(int addr, Builder *Maker):Instruction(addr,"0x4E",0x4E,Maker){}
    OPCode4E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x4E", 0x4E,Maker){
            addr++;
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
    }
};
class OPCode50 : public Instruction
{
    public:
    OPCode50():Instruction(-1,0x50,nullptr){}
    OPCode50(int addr, Builder *Maker):Instruction(addr,"0x50",0x50,Maker){}
    OPCode50(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x50", 0x50,Maker){
            addr++;
            fun_1403c90e0(addr, content, this, 1);

    }
};
class OPCode53 : public Instruction
{
    public:
    OPCode53():Instruction(-1,0x53,nullptr){}
    OPCode53(int addr, Builder *Maker):Instruction(addr,"0x53",0x53,Maker){}
    OPCode53(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x53", 0x53,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));


    }
};
class OPCode54 : public Instruction
{
    public:
    OPCode54():Instruction(-1,0x54,nullptr){}
    OPCode54(int addr, Builder *Maker):Instruction(addr,"???",0x54,Maker){}
    OPCode54(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x54,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            QByteArray control_byte2;
            this->AddOperande(operande(addr,"byte", control_byte));
            switch ((unsigned char)control_byte[0])  {
                case 0x1:
                case 0x3:
                case 0x8:
                case 0x0:
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr,0x20-4)));
                    break;
                case 10:
                case 0xE:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                case 0xB:

                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0xD:

                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x14:

                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x15:

                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                    break;
                case 0x17:
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                case 0x24:
                case 0x22:
                case 0x18:
                case 0x28:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x19:
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                case 0x1C:
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x21:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;

                case 0x23:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x26:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

                    break;

                case 0x29:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 0x2B:
                    control_byte2 = ReadSubByteArray(content, addr, 1);
                    this->AddOperande(operande(addr,"byte", control_byte2));
                    if ((int)control_byte2[0]==0){
                        this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));}
                    break;
                case 0x2C:

                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x31:
                    control_byte2 = ReadSubByteArray(content, addr, 1);
                    this->AddOperande(operande(addr,"byte", control_byte2));
                    if ((int)control_byte2[0]==0){
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    }
                    else if ((int)control_byte2[0]==1){
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    }
                    break;
            case 0x34:

                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;

            case 0x35:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            case 0x37:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            case 0x38:

                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            case 0x39:

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 0x3A:

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2))); //0
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2))); //2
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//4
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//8
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//C
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//10
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1))); //0x14

                break;
            case 0x3B:

                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2))); //0
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2))); //2
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//4
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));//8
                break;

            case 0x3C:

                this->AddOperande(operande(addr,"bytearray", ReadSubByteArray(content, addr,0xA)));
                break;
            case 0x3E:
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;

            case 0x40:
            case 0x4B:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 0x42:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 0x44:
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            case 0x45:
            case 0x46:
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;

            case 0x47:
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 0x48:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            case 0x49:
            case 0x4A:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 0x4C:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            case 0x4D:
                this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                break;
            case 0x4F:
                control_byte2 = ReadSubByteArray(content, addr, 1);
                this->AddOperande(operande(addr,"byte", control_byte2)); //1
                switch((int)control_byte2[0]){
                    case 0:
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        break;
                    case 1:
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                        break;
                    case 2:
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));

                        break;
                    case 3:
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case 4:
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                        break;
                    case '\n':
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2))); //3
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1))); //4
                        this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4))); //8
                        this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2))); //10
                        break;
                }
                break;
            case 0x50:
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            case 0x51:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 0x52:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));


                break;
            case 0x53:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;

            }


    }
};
class OPCode55 : public Instruction
{
    public:
    OPCode55():Instruction(-1,0x55,nullptr){}
    OPCode55(int addr, Builder *Maker):Instruction(addr,"0x55",0x55,Maker){}
    OPCode55(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x55", 0x55,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));

    }
};
class OPCode56 : public Instruction
{
    public:
    OPCode56():Instruction(-1,0x56,nullptr){}
    OPCode56(int addr, Builder *Maker):Instruction(addr,"0x56",0x56,Maker){}
    OPCode56(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x56", 0x56,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//8
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//C
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//10
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));//14
    }
};
class OPCode57 : public Instruction
{
    public:
    OPCode57():Instruction(-1,0x57,nullptr){}
    OPCode57(int addr, Builder *Maker):Instruction(addr,"0x57",0x57,Maker){}
    OPCode57(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x57", 0x57,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
    }
};
class OPCode58 : public Instruction
{
    public:
    OPCode58():Instruction(-1,0x58,nullptr){}
    OPCode58(int addr, Builder *Maker):Instruction(addr,"0x58",0x58,Maker){}
    OPCode58(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x58", 0x58,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
    }
};
class OPCode5C : public Instruction
{
    public:
    OPCode5C():Instruction(-1,0x5C,nullptr){}
    OPCode5C(int addr, Builder *Maker):Instruction(addr,"???",0x5C,Maker){}
    OPCode5C(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x5C,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
    }
};
class OPCode5E : public Instruction
{
    public:
    OPCode5E():Instruction(-1,0x5E,nullptr){}
    OPCode5E(int addr, Builder *Maker):Instruction(addr,"???",0x5E,Maker){}
    OPCode5E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x5E,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            switch((unsigned char)control_byte[0]){
                case 0:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    break;
                case 1:
                    break;
            }

    }
};
class OPCode62 : public Instruction
{
    public:
    OPCode62():Instruction(-1,0x62,nullptr){}
    OPCode62(int addr, Builder *Maker):Instruction(addr,"???",0x62,Maker){}
    OPCode62(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x62,Maker){
            addr++;
    }
};
class OPCode63 : public Instruction
{
    public:
    OPCode63():Instruction(-1,0x63,nullptr){}
    OPCode63(int addr, Builder *Maker):Instruction(addr,"???",0x63,Maker){}
    OPCode63(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x63,Maker){
            addr++;
            QByteArray control_short = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr,"short", control_short));
            short control_shrt = ReadShortFromByteArray(0, control_short);
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

    }
};
class OPCode66 : public Instruction
{
    public:
    OPCode66():Instruction(-1,0x66,nullptr){}
    OPCode66(int addr, Builder *Maker):Instruction(addr,"???",0x66,Maker){}
    OPCode66(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x66,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            switch((unsigned char)control_byte[0]){
                case 0:
                case 1:
                case 4:
                case 8:
                case 6:
                case 9:
                case 10:
                case 0xC:
                case 0xE:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 2:
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;

            }
    }
};
class OPCode68 : public Instruction
{
    public:
    OPCode68():Instruction(-1,0x68,nullptr){}
    OPCode68(int addr, Builder *Maker):Instruction(addr,"???",0x68,Maker){}
    OPCode68(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x68,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            switch((unsigned char)control_byte[0]){
            case 0:
            case 1:
            case '\v':
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
            case 2:
            case 3:
            case 4:
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            case '\a':
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                break;
            case '\b':
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                break;
            case '\x18':
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            case '\x1E':
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            }
    }
};
class OPCode69 : public Instruction
{
    public:
    OPCode69():Instruction(-1,0x69,nullptr){}
    OPCode69(int addr, Builder *Maker):Instruction(addr,"???",0x69,Maker){}
    OPCode69(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x69,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                case 1:
                case 4:
                case 7:
                case 8:
                case 9:
                case 0xB:
                case 0x13:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 2:
                case 3:
                case 5:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 10:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    break;
                case 0xC:
                case 0xD:
                case 0x12:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;

            }
    }
};
class OPCode6A : public Instruction
{
    public:
    OPCode6A():Instruction(-1,0x6A,nullptr){}
    OPCode6A(int addr, Builder *Maker):Instruction(addr,"???",0x6A,Maker){}
    OPCode6A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x6A,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0:
                case 3:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1))); //not sure?
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                    break;
                case 2:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                    break;
                case 4:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 5:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

                    break;

            }
    }
};
class OPCode6C : public Instruction
{
    public:
    OPCode6C():Instruction(-1,0x6C,nullptr){}
    OPCode6C(int addr, Builder *Maker):Instruction(addr,"???",0x6C,Maker){}
    OPCode6C(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x6C,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));

    }
};
class OPCode6E : public Instruction
{
    public:
    OPCode6E():Instruction(-1,0x6E,nullptr){}
    OPCode6E(int addr, Builder *Maker):Instruction(addr,"???",0x6E,Maker){}
    OPCode6E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x6E,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
            this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));

    }
};
class OPCode6F : public Instruction
{
    public:
    OPCode6F():Instruction(-1,0x6F,nullptr){}
    OPCode6F(int addr, Builder *Maker):Instruction(addr,"0x6F",0x6F,Maker){}
    OPCode6F(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x6F", 0x6F,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));

    }
};
class OPCode70 : public Instruction
{
    public:
    OPCode70():Instruction(-1,0x70,nullptr){}
    OPCode70(int addr, Builder *Maker):Instruction(addr,"???",0x70,Maker){}
    OPCode70(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x70,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            switch((unsigned char)control_byte[0]){
                case 0:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 1:
                case 7:
                case 0xB:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 4:
                case 9:
                case 0xC:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 2:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 3:
                case 5:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 6:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 8:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 10:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
            }
    }
};
class OPCode72 : public Instruction // not sure at all
{
    public:
    OPCode72():Instruction(-1,0x72,nullptr){}
    OPCode72(int addr, Builder *Maker):Instruction(addr,"???",0x72,Maker){}
    OPCode72(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x72,Maker){
            addr++;
            QByteArray control_short = ReadSubByteArray(content, addr,2);
            this->AddOperande(operande(addr,"short", control_short));
            ushort control_shrt = ReadShortFromByteArray(0, control_short);
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            if ((unsigned char) (control_byte[0] - 1U) < 2) {
              this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
              this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            }
            else {
              if ((unsigned char)(control_byte[0] - 3U) < 2) {
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
              }
            }
            if (control_shrt != 0xFFFF){
                bool bVar9 = control_shrt != 0x270e;
                if (control_shrt<0x100){
                    switch((unsigned char) control_byte[0]){
                    case '\x06':
                        this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                        break;

                    }
                }
            }
    }
};
class OPCode74 : public Instruction
{
    public:
    OPCode74():Instruction(-1,0x74,nullptr){}
    OPCode74(int addr, Builder *Maker):Instruction(addr,"???",0x74,Maker){}
    OPCode74(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x74,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            switch((unsigned char)control_byte[0]){
                case 0:
                case 0x14:
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                case 2:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//0
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//2
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//6
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));//A
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    break;
                case 3:
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

                    break;
            }
    }
};
class OPCode77 : public Instruction
{
    public:
    OPCode77():Instruction(-1,0x77,nullptr){}
    OPCode77(int addr, Builder *Maker):Instruction(addr,"???",0x77,Maker){}
    OPCode77(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x77,Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

    }
};
class OPCode7B : public Instruction
{
    public:
    OPCode7B():Instruction(-1,0x7B,nullptr){}
    OPCode7B(int addr, Builder *Maker):Instruction(addr,"???",0x7B,Maker){}
    OPCode7B(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x7B,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            switch((unsigned char)control_byte[0]){

                case 1:
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
            }
    }
};
class OPCode7E : public Instruction
{
    public:
    OPCode7E():Instruction(-1,0x7E,nullptr){}
    OPCode7E(int addr, Builder *Maker):Instruction(addr,"???",0x7E,Maker){}
    OPCode7E(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"???", 0x7E,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));

            switch((unsigned char)control_byte[0]){
                case 0:
                case 1:

                    break;
                case 2:
                case 6:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
                case 3:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
                case 4:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                break;
                case 5:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;

            }
    }
};
class OPCode86 : public Instruction
{
    public:
    OPCode86():Instruction(-1,0x86,nullptr){}
    OPCode86(int addr, Builder *Maker):Instruction(addr,"0x86",0x86,Maker){}
    OPCode86(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x86", 0x86,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//2
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//4
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//6
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//8
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//A
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//c
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));


    }
};
class OPCode8A : public Instruction
{
    public:
    OPCode8A():Instruction(-1,0x8A,nullptr){}
    OPCode8A(int addr, Builder *Maker):Instruction(addr,"0x8A",0x8A,Maker){}
    OPCode8A(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x8A", 0x8A,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
            case 0:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            case 1: //NOT SURE!
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                break;
            case 2: //NOT SURE!
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));

                break;
            case 3:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            case -2:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            case -1:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
            default:
                if (((unsigned char)control_byte[0] - 0x32U) < 2){
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                }
                break;
            }





    }
};
class OPCode98 : public Instruction
{
    public:
    OPCode98():Instruction(-1,0x98,nullptr){}
    OPCode98(int addr, Builder *Maker):Instruction(addr,"0x98",0x98,Maker){}
    OPCode98(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x98", 0x98,Maker){
            addr++;
            QByteArray control_short = ReadSubByteArray(content, addr, 2);
            this->AddOperande(operande(addr,"short", control_short));
            short control_shrt = ReadShortFromByteArray(0, control_short);
            switch(control_shrt){
             case 1:
             case 2:
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                break;
             case 3:
                this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
             case 0x3E8:
             case 0x3E9:
                this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr,4)));
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr,1)));
                break;
            }
            /*TO REMOVE
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//4
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//6
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//8
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//A
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));//c
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));*/


    }
};
class OPCode90 : public Instruction
{
    public:
    OPCode90():Instruction(-1,0x90,nullptr){}
    OPCode90(int addr, Builder *Maker):Instruction(addr,"0x90",0x90,Maker){}
    OPCode90(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x90", 0x90,Maker){
            addr++;
            this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));

    }
};
class OPCode93 : public Instruction
{
    public:
    OPCode93():Instruction(-1,0x93,nullptr){}
    OPCode93(int addr, Builder *Maker):Instruction(addr,"0x93",0x93,Maker){}
    OPCode93(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x93", 0x93, Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
            case 0:
                this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                break;
            }

    }
};
class OPCode99 : public Instruction
{
    public:
    OPCode99():Instruction(-1,0x99,nullptr){}
    OPCode99(int addr, Builder *Maker):Instruction(addr,"0x99",0x99,Maker){}
    OPCode99(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0x99", 0x99, Maker){
            addr++;
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));

    }
};
class OPCodeAC : public Instruction
{
    public:
    OPCodeAC():Instruction(-1,0xAC,nullptr){}
    OPCodeAC(int addr, Builder *Maker):Instruction(addr,"0xAC",0xAC,Maker){}
    OPCodeAC(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0xAC", 0xAC,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x0:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x2:
                case 0x1:
                    //here we're calling a function that was defined at the beginning
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr,4)));
                    break;
                case 0x5:
                case '\a':
                case '\t':
                    //here we're calling a function that was defined at the beginning
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr,2)));
                    break;
            }

    }
};
class OPCodeAF : public Instruction
{
    public:
    OPCodeAF():Instruction(-1,0xAF,nullptr){}
    OPCodeAF(int addr, Builder *Maker):Instruction(addr,"0xAF",0xAF,Maker){}
    OPCodeAF(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0xAF", 0xAF,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
    }
};
class OPCodeB1 : public Instruction
{
    public:
    OPCodeB1():Instruction(-1,0xB1,nullptr){}
    OPCodeB1(int addr, Builder *Maker):Instruction(addr,"0xB1",0xB1,Maker){}
    OPCodeB1(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0xB1", 0xB1,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
            this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));

    }
};
class OPCodeC0 : public Instruction
{
    public:
    OPCodeC0():Instruction(-1,0xC0,nullptr){}
    OPCodeC0(int addr, Builder *Maker):Instruction(addr,"0xC0",0xC0,Maker){}
    OPCodeC0(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0xC0", 0xC0,Maker){
            addr++;
            QByteArray control_short = ReadSubByteArray(content, addr, 2);
            short control_shrt = ReadShortFromByteArray(0, control_short);
            this->AddOperande(operande(addr,"short", control_short));
            switch(control_shrt){
                case 0x2:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x3:
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;
                case 0x4:
                    this->AddOperande(operande(addr,"string", ReadStringSubByteArray(content, addr)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                default:
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    break;


            }

    }
};
class OPCodeC4 : public Instruction
{
    public:
    OPCodeC4():Instruction(-1,0xC4,nullptr){}
    OPCodeC4(int addr, Builder *Maker):Instruction(addr,"0xC4",0xC4,Maker){}
    OPCodeC4(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0xC4", 0xC4,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x0:

                    break;
                case 0x1:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;
                case 0x2:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x3:
                    this->AddOperande(operande(addr,"short", ReadSubByteArray(content, addr, 2)));
                    break;
                case 0x4:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    fun_1403c90e0(addr, content, this, 0);

                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));

                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));
                    this->AddOperande(operande(addr,"float", ReadSubByteArray(content, addr, 4)));

                    break;
                case 0x5:
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    this->AddOperande(operande(addr,"byte", ReadSubByteArray(content, addr, 1)));
                    break;

            }

    }
};
class OPCodeCA : public Instruction
{
    public:
    OPCodeCA():Instruction(-1,0xCA,nullptr){}
    OPCodeCA(int addr, Builder *Maker):Instruction(addr,"0xCA",0xCA,Maker){}
    OPCodeCA(int &addr, QByteArray &content, Builder *Maker):Instruction(addr,"0xCA", 0xCA,Maker){
            addr++;
            QByteArray control_byte = ReadSubByteArray(content, addr, 1);
            this->AddOperande(operande(addr,"byte", control_byte));
            switch((unsigned char)control_byte[0]){
                case 0x0:
                    this->AddOperande(operande(addr,"int", ReadSubByteArray(content, addr, 4)));
                    break;

            }

    }
};
class CS3TranslationFile : public TranslationFile
{
    public:
    CS3TranslationFile():TranslationFile(){}

};
class CS3Builder : public Builder
{
    public:
    CS3Builder(){

    }
    std::shared_ptr<Instruction> CreateInstructionFromXLSX(int row){
        return std::make_shared<OPCode0>();
    }
    std::shared_ptr<Instruction> CreateInstructionFromDAT(int &addr, QByteArray &dat_content, int function_type){
        int OP = (dat_content[addr]&0xFF);

        if (function_type == 0){ //the function is read with OPCodes


            switch(OP){
                case 0x00: return std::make_shared<OPCode0>(addr,dat_content,this);
                case 0x01: return std::make_shared<OPCode1>(addr,dat_content,this);
                case 0x02: return std::make_shared<OPCode2>(addr,dat_content,this);
                case 0x03: return std::make_shared<OPCode3>(addr,dat_content,this);
                case 0x04: return std::make_shared<OPCode4>(addr,dat_content,this);
                case 0x05: return std::make_shared<OPCode5>(addr,dat_content,this);
                case 0x07: return std::make_shared<OPCode7>(addr,dat_content,this);
                case 0x08: return std::make_shared<OPCode8>(addr,dat_content,this);
                case 0x0C: return std::make_shared<OPCodeC>(addr,dat_content,this);
                case 0x10: return std::make_shared<OPCode10>(addr,dat_content,this);
                case 0x11: return std::make_shared<OPCode11>(addr,dat_content,this);
                case 0x13: return std::make_shared<OPCode13>(addr,dat_content,this);
                case 0x14: return std::make_shared<OPCode14>(addr,dat_content,this);
                case 0x16: return std::make_shared<OPCode16>(addr,dat_content,this);
                case 0x18: return std::make_shared<OPCode18>(addr,dat_content,this);
                case 0x1D: return std::make_shared<OPCode1D>(addr,dat_content,this);
                case 0x1E: return std::make_shared<OPCode1E>(addr,dat_content,this);
                case 0x1F: return std::make_shared<OPCode1F>(addr,dat_content,this);
                case 0x20: return std::make_shared<OPCode20>(addr,dat_content,this);
                case 0x21: return std::make_shared<OPCode21>(addr,dat_content,this);
                case 0x22: return std::make_shared<OPCode22>(addr,dat_content,this);
                case 0x23: return std::make_shared<OPCode23>(addr,dat_content,this);
                case 0x24: return std::make_shared<OPCode24>(addr,dat_content,this);
                case 0x25: return std::make_shared<OPCode25>(addr,dat_content,this);
                case 0x26: return std::make_shared<OPCode26>(addr,dat_content,this);
                case 0x27: return std::make_shared<OPCode27>(addr,dat_content,this);
                case 0x2B: return std::make_shared<OPCode2B>(addr,dat_content,this);
                case 0x2F: return std::make_shared<OPCode2F>(addr,dat_content,this);
                case 0x32: return std::make_shared<OPCode32>(addr,dat_content,this);
                case 0x35: return std::make_shared<OPCode35>(addr,dat_content,this);
                case 0x36: return std::make_shared<OPCode36>(addr,dat_content,this);
                case 0x37: return std::make_shared<OPCode37>(addr,dat_content,this);
                case 0x38: return std::make_shared<OPCode38>(addr,dat_content,this);
                case 0x39: return std::make_shared<OPCode39>(addr,dat_content,this);
                case 0x3A: return std::make_shared<OPCode3A>(addr,dat_content,this);
                case 0x3B: return std::make_shared<OPCode3B>(addr,dat_content,this);
                case 0x3C: return std::make_shared<OPCode3C>(addr,dat_content,this);
                case 0x3D: return std::make_shared<OPCode3D>(addr,dat_content,this);
                case 0x3E: return std::make_shared<OPCode3E>(addr,dat_content,this);
                case 0x3F: return std::make_shared<OPCode3F>(addr,dat_content,this);
                case 0x40: return std::make_shared<OPCode40>(addr,dat_content,this);
                case 0x41: return std::make_shared<OPCode41>(addr,dat_content,this);
                case 0x42: return std::make_shared<OPCode42>(addr,dat_content,this);
                case 0x43: return std::make_shared<OPCode43>(addr,dat_content,this);
                case 0x44: return std::make_shared<OPCode44>(addr,dat_content,this);
                case 0x45: return std::make_shared<OPCode45>(addr,dat_content,this);
                case 0x46: return std::make_shared<OPCode46>(addr,dat_content,this);
                case 0x47: return std::make_shared<OPCode47>(addr,dat_content,this);
                case 0x48: return std::make_shared<OPCode48>(addr,dat_content,this);
                case 0x49: return std::make_shared<OPCode49>(addr,dat_content,this);
                case 0x4B: return std::make_shared<OPCode4B>(addr,dat_content,this);
                case 0x4E: return std::make_shared<OPCode4E>(addr,dat_content,this);
                case 0x50: return std::make_shared<OPCode50>(addr,dat_content,this);
                case 0x53: return std::make_shared<OPCode53>(addr,dat_content,this);
                case 0x54: return std::make_shared<OPCode54>(addr,dat_content,this);
                case 0x55: return std::make_shared<OPCode55>(addr,dat_content,this);
                case 0x56: return std::make_shared<OPCode56>(addr,dat_content,this);
                case 0x57: return std::make_shared<OPCode57>(addr,dat_content,this);
                case 0x58: return std::make_shared<OPCode58>(addr,dat_content,this);
                case 0x5C: return std::make_shared<OPCode5C>(addr,dat_content,this);
                case 0x5E: return std::make_shared<OPCode5E>(addr,dat_content,this);
                case 0x62: return std::make_shared<OPCode62>(addr,dat_content,this);
                case 0x63: return std::make_shared<OPCode63>(addr,dat_content,this);
                case 0x66: return std::make_shared<OPCode66>(addr,dat_content,this);
                case 0x68: return std::make_shared<OPCode68>(addr,dat_content,this);
                case 0x69: return std::make_shared<OPCode69>(addr,dat_content,this);
                case 0x6A: return std::make_shared<OPCode6A>(addr,dat_content,this);
                case 0x6C: return std::make_shared<OPCode6C>(addr,dat_content,this);
                case 0x6E: return std::make_shared<OPCode6E>(addr,dat_content,this);
                case 0x6F: return std::make_shared<OPCode6F>(addr,dat_content,this);
                case 0x70: return std::make_shared<OPCode70>(addr,dat_content,this);
                case 0x72: return std::make_shared<OPCode72>(addr,dat_content,this);
                case 0x74: return std::make_shared<OPCode74>(addr,dat_content,this);
                case 0x77: return std::make_shared<OPCode77>(addr,dat_content,this);
                case 0x7B: return std::make_shared<OPCode7B>(addr,dat_content,this);
                case 0x7E: return std::make_shared<OPCode7E>(addr,dat_content,this);
                case 0x86: return std::make_shared<OPCode86>(addr,dat_content,this);
                case 0x8A: return std::make_shared<OPCode8A>(addr,dat_content,this);
                case 0x90: return std::make_shared<OPCode90>(addr,dat_content,this);
                case 0x93: return std::make_shared<OPCode93>(addr,dat_content,this);
                case 0x98: return std::make_shared<OPCode98>(addr,dat_content,this);
                case 0x99: return std::make_shared<OPCode99>(addr,dat_content,this);
                case 0xAC: return std::make_shared<OPCodeAC>(addr,dat_content,this);
                case 0xAF: return std::make_shared<OPCodeAF>(addr,dat_content,this);
                case 0xB1: return std::make_shared<OPCodeB1>(addr,dat_content,this);
                case 0xC0: return std::make_shared<OPCodeC0>(addr,dat_content,this);
                case 0xC4: return std::make_shared<OPCodeC4>(addr,dat_content,this);
                case 0xCA: return std::make_shared<OPCodeCA>(addr,dat_content,this);
                default:
                std::stringstream stream;
                stream << "L'OP code " << std::hex << OP << " n'est pas défini !!";
                std::string result( stream.str() );

                qFatal(result.c_str());

                return std::shared_ptr<Instruction>();
            }
        }
        else if (function_type==1){//the function is a "CreateMonsters" function
            return std::make_shared<CreateMonsters>(addr,dat_content,this);
        }
        else if (function_type==2){//the function is a "effect" function
            return std::make_shared<EffectsInstr>(addr,dat_content,this);
        }

    }
    bool CreateHeaderFromDAT(QByteArray &dat_content){
        //Header structure:
        //The first 0x20 I'd say is probably the position of the name of the file; which shouldn't change
        //The second 0x20 probably doesn't change too.
        //The next integer is the position of the first pointer
        //The 4th: probably the length in bytes of the pointer section
        //The fifth: probably the position of the "names positions" section (right after the pointer section)
        //The sixth: the number of functions inside the file
        //the seventh: the position one byte after the 0x00 separator at the end of the functions section
        //the eighth: 0xABCDEF00 => seems to always be there (no idea why)
        //Then the name of the file
        //Then the pointer section
        //Then the "names positions" section
        //Then the functions section
        //Done; here the function objects holds the number of functions, the addr, name positions
        //everything else can be deduced
        int nb_functions = dat_content[0x14];
        int position = 0x20;
        QString filename = ReadStringFromByteArray(position, dat_content);
        SceneName = filename;
        int start_offset_area = 0x20 + filename.length()+1;
        for (int idx_fun = 0; idx_fun < nb_functions; idx_fun++){
            position = start_offset_area + 4*idx_fun;
            int addr = ReadIntegerFromByteArray(position, dat_content);
            position = start_offset_area + 4*nb_functions + 2 * idx_fun;
            short name_pos = ReadShortFromByteArray(position, dat_content);
            int name_pos_int = name_pos;
            QString function_name = ReadStringFromByteArray(name_pos_int, dat_content);
            FunctionsToParse.push_back(function(idx_fun,function_name,name_pos,addr));
        }
        return true;
    }
    bool CreateHeaderFromXLSX(){
        return true;
    }
};


#endif // CS3INSTRUCTIONSSET_H