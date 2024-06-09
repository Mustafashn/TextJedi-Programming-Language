// 05210000275 Mustafa Şahin
// 05210000948 Simge Cengiz
/* cmd istemcisinden TextJedi myProg.tj yazdığında çalıştırılır
 * CLion terminalinden TextJedi myProg.tj yazdığında çalıştırılır,
 * location hatası olması durumunda ./TextJedi myProg.tj olarak çalıştırılır bu dizinde olduğunu kesin belirtir
 * */
// tanımlamalarda bosluklara dikkat edilmelidir örn: myInt := 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_IDENTIFIER_SIZE 30
#define MAX_STRING_SIZE 100
//Öncelikle struct tanımlamaları yapıldı
typedef struct {
    char name[MAX_IDENTIFIER_SIZE + 1];
    char value[MAX_STRING_SIZE + 1];
} TextVariable;

typedef struct {
    char name[MAX_IDENTIFIER_SIZE + 1];
    int value;
} IntVariable;

typedef struct {
    char text[MAX_STRING_SIZE + 1];
} TextConstant;

// Function declarations
char* findVariable(char* name);
char* findVariableType(char* name);
char* findVariableValueText(char* name);
int findVariableValueInt(char* name);
char* is_functionKeywordFunction (char*name);
char* is_commandKeywordFunction (char*name);
void newVariable(char* type, char* name);
void assignValue(char* variableName, char* value);
void performOperationIntVariable(char* variableName, char* operation, char* operand1, char* operand2);
void performOperationDigital(char* variableName,char* operation,char* operand1 ,int operand2 );
void performOperationTextVariable(char* variableName, char* operation, char* operand1, char* operand2);
void performOperationString(char* variableName, char* operation, char* operand1, char* operand2);
int subtractSubstring(char* minuend, const char* subtrahend);
int locateSubstring(char* bigText, char* smallText, int start);
int locate(char* bigText,char* smallText,int start);
char* insert(char* myText,int location,char* insert);
char* override(char* myText, int location, char* ovrText);
void readFromFile(char* variableName, char* fileName);
void writeToFile(char* variableName, char* fileName);
void takeInput(char* variableName, char* promptText);
void outputText(char* variableName);
char* subsText(char* text, int begin, int end);
char* insertText(char* text, int location, char* insertText);
char* overrideText(char* text, int location, char* ovrText);
char* asString(int number);
int asText(char* text);
int size(char* myText);
char* subs(char* myText,int begin,int end);

// Global variables
TextVariable textVariables[MAX_IDENTIFIER_SIZE];
IntVariable intVariables[MAX_IDENTIFIER_SIZE];
int numTextVariables = 0;
int numIntVariables = 0;
FILE *file;
char file_line[250];
char* token;
char function_keywords [8][15] ={"size","subs","locate","insert","override","asString","asText"};
char commands_keywords [6][15] ={"read","write","output","input"};
int main(int argc, char* argv[]) {

      if (argc != 2) {
        printf("Usage: TextJedi <filename>\n");
        return 1;
    }
    // Extract the filename from the command-line argument
    const char* filename = argv[1];

    // Run the TextJedi program
    file = fopen("myProg.tj","r");
    if(file == NULL) {
        printf("This file is null");
    }

    const char outer_delimiters[] = " \";(),";

    char* outer_saveptr = NULL;
    bool is_commentLineOpen =false;
    while(fgets(file_line,sizeof (file_line),file)!=NULL) {
        token = strtok(file_line,outer_delimiters);
        int i=0;
        bool is_new=false;
        char* is_int=""; /* int için int text için text olustur "" bosluk için hata döndürür */
        char* is_mathOp=""; /* + ve - için bir sonraki adımda işlem yapılır */
        char* is_variable_in_array="";
        char* operand="";
        int is_error =0;
        char* variable_type="";
        char* is_funcKeyword ="";
        char* is_commandKeyword ="";
        char* funcParameter1 ="";
        char* funcParameter2 ="";
        char* funcParameter3 ="";
        char* commandParameter1 ="";
        char* commandParameter2 ="";
        char* commandConst =""; // Commandslar için keywords dışında kalan sabit kelime ataması için

        while (token != NULL) {
            char* temp_token=token;
            // commentline kontrolu icin
            if (strcmp(token,"*/\n") ==0 || strcmp(token,"*/")==0) {
                is_commentLineOpen=false;
            }
            else if (strcmp(token,"/*") ==0 || strcmp(token,"/*\n") ==0) {
                is_commentLineOpen=true;
            }
            // Yorum satırı değilse okunacak kod için kontrolü
            if (is_commentLineOpen == false) {
                //Yeni bir değişken oluşturma tokeni
                if (strcmp(token,"new")==0 && i ==0) {
                    is_new=true;
                }
                else if (i == 0) {
                    //Degisken tanımlı mı ve tipi kontrol ediliyor
                    //Degisken degilse commandKeyword ve funcKeyword kontrolü yapılıyor
                    is_variable_in_array= findVariable(token);
                    variable_type= findVariableType(is_variable_in_array);
                    if (strcmp(is_variable_in_array,"") == 0) {
                        is_funcKeyword = is_functionKeywordFunction(token);
                        is_commandKeyword = is_commandKeywordFunction(token);
                    }
                }
                //funcKeyword ise çağırılması için bloklar
                else if (i ==1 && strcmp(is_funcKeyword,"") !=0) {
                    funcParameter1 = token;
                    if (strcmp(is_funcKeyword,"size")==0) {
                        int return_size=size(funcParameter1);
                        printf("return size %d \n",return_size);
                    }
                    else if (strcmp(is_funcKeyword,"asString")==0) {
                        char* result_string = asString(findVariableValueInt(funcParameter1));
                        printf("result string : %s \n",result_string);
                    }
                    else if (strcmp(is_funcKeyword,"asText")==0) {
                        int result_int = asText(findVariableValueText(funcParameter1));
                        printf("result int : %d \n",result_int);
                    }
                }
                    //commandKeyword ise çağırılması için bloklar
                else if (i ==1 && strcmp(is_commandKeyword,"") != 0) {
                    commandParameter1 = token;
                    if (strcmp(is_commandKeyword,"output") ==0) {
                        outputText(token);
                    }
                }
                //commandKeywords ve funcKeywords için atanması gereken parametreler atanır
                else if (i == 2 && strcmp(is_commandKeyword,"") != 0) {
                    commandConst=token;
                }
                else if (i ==2 && strcmp(is_funcKeyword,"") !=0) {
                    funcParameter2 = token;
                }
                else if (i ==3 && strcmp(is_funcKeyword,"") !=0) {
                    funcParameter3 = token;
                    if (strcmp(is_funcKeyword,"subs")==0) {
                        char* subs_result=subs(funcParameter1,atoi(funcParameter2), atoi(funcParameter3));
                        printf("substring : %s \n",subs_result);
                    }
                    else if (strcmp(is_funcKeyword,"locate")==0) {
                        //LOCATE BAK ÇALIŞMIYOR
                        int locate_result = locate(funcParameter1, findVariableValueText(funcParameter2),atoi(funcParameter3));
                        printf("locate : %d \n",locate_result);
                    }
                    else if (strcmp(is_funcKeyword,"insert")==0) {
                        char* insert_result = insertText(findVariableValueText(funcParameter1), atoi(funcParameter2),findVariableValueText(funcParameter3));
                        printf("insert : %s \n",insert_result);
                    }
                    else if (strcmp(is_funcKeyword,"override")==0) {
                        char* override_result = override(funcParameter1, atoi(funcParameter2), findVariableValueText(funcParameter3));
                        printf("override : %s \n",override_result);
                    }
                }
                else if (i == 3 && strcmp(is_commandKeyword,"") != 0) {
                    commandParameter2 = token;

                    if (strcmp(is_commandKeyword,"read")==0 && strcmp(commandConst,"from") ==0) {
                        readFromFile(commandParameter1,commandParameter2);
                    }
                    else if (strcmp(is_commandKeyword,"write")== 0 && strcmp(commandConst,"to") ==0) {

                        writeToFile(commandParameter1,commandParameter2);
                    }
                    else if (strcmp(is_commandKeyword,"input")==0 && strcmp(commandConst,"prompt") ==0) {
                        takeInput(commandParameter1,commandParameter2);
                    }
                }
                else if (strcmp(token,"int")==0 && i ==1) {
                    is_int="int";
                }
                else if (strcmp(token,"text")==0 && i ==1) {
                    is_int="text";
                }
                // olusturulan degisken icin deger ataması yapılıyor
                else if (i == 2 && (strcmp(is_variable_in_array,"")!=0)) {
                    int degerInt= findVariableValueInt(token);
                    char* degerText= findVariableValueText(token);
                    if (strcmp(degerText,"") !=0) {
                        assignValue(is_variable_in_array,degerText);
                    }
                    else if (degerInt!= -1) {
                        assignValue(is_variable_in_array, asString(degerInt));
                    }
                    else {
                        assignValue(is_variable_in_array,token);
                    }
                }
                // degisken olusturuluyor
                else if (i ==2 && is_new == true) {
                    if (strcmp(is_int,"int") ==0) {
                        newVariable("int",token);
                    }
                    else if (strcmp(is_int,"text")==0 ){
                        newVariable("text",token);
                    }
                    else {
                        printf("Bu tipte bir degisken olusturulamaz !");
                    }
                }
                //operator islemleri icin
                else if (i == 3 &&( strcmp(token,"+") == 0 || strcmp(token,"-") == 0 )) {
                    if (strcmp(token,"+") == 0) {
                        is_mathOp="+";
                    }
                    else {
                        is_mathOp="-";
                    }
                }
                else if (i ==4 && strcmp(is_mathOp,"") != 0 ) {
                    operand=token;
                    /* islem yapılacak değişkenin olup olmadığı kontrol edilir */
                    char* temp =findVariable(operand);
                    if (strcmp(is_mathOp,"+") ==0) {

                        if (strcmp(variable_type,"int") ==0) {
                            /* int değerle toplanır direkt */
                            if ( atoi(operand) >=0 && strcmp(temp,"")==0) {
                                performOperationDigital(is_variable_in_array, is_mathOp, is_variable_in_array, atoi(operand) );
                            }
                                /* degiskenden değer alınır */
                            else {

                                performOperationIntVariable(is_variable_in_array, is_mathOp, is_variable_in_array, operand );
                            }

                        }
                            /* string islem */
                        else {

                            if (strcmp(temp,"") ==0) {
                                performOperationString(is_variable_in_array, is_mathOp, is_variable_in_array, operand );
                            }
                            else {
                                performOperationTextVariable(is_variable_in_array, is_mathOp, is_variable_in_array, operand );
                            }

                        }
                    }
                        /* is_Math - ise */
                    else {
                        //text için
                        if (strcmp(variable_type,"text") ==0) {
                            char* subString = operand;
                            if (strcmp(temp,"") !=0) {
                                subString = findVariableValueText(operand);
                            }
                            is_error =subtractSubstring(findVariableValueText(is_variable_in_array), subString);
                        }
                            //int için
                        else {
                            if ( atoi(operand) >=0 && strcmp(temp,"")==0) {
                                performOperationDigital(is_variable_in_array, is_mathOp, is_variable_in_array, atoi(operand) );
                            }
                                /* degiskenden değer alınır */
                            else {

                                performOperationIntVariable(is_variable_in_array, is_mathOp, is_variable_in_array, operand );
                            }
                        }
                    }
                    //subtraction için hata kodları
                    if (is_error == -1) {
                        printf("ERROR : Substring is bigger than first string! \n");
                    }
                    else if (is_error == -2) {
                        printf("ERROR: No substring found in first string! \n");
                    }

                }
                else if (strcmp(token,"\n") ==0) {
                    //  printf("token bos");
                }
                else {
                }
            }
            i++;

            token = strtok(NULL, outer_delimiters);
        }
        is_new=false;
        is_int="";

    }

    fclose(file);
    return 0;

    return 0;
}
char* is_functionKeywordFunction (char*name) {
    for (int i = 0; i< 7  ; ++i) {
        if (!strcmp(name,function_keywords[i])) {
            return name;
        }
    }
    return "";
}
char* is_commandKeywordFunction (char*name) {
    for (int i = 0; i<4  ; ++i) {
        if (!strcmp(name,commands_keywords[i])) {
            return name;
        }
    }
    return "";
}
char* findVariable(char* name) {
        for (int i = 0; i < numTextVariables; i++) {
            if (strcmp(textVariables[i].name, name) == 0) {
                return name;
            }
        }
        for (int k = 0; k < numIntVariables; k++) {
            if (strcmp(intVariables[k].name, name) == 0) {
                return name;
            }
        }
        return "";
    }

char* findVariableValueText(char* name) {
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, name) == 0) {
            return textVariables[i].value;
        }
    }
    return "";
}
int  findVariableValueInt(char* name) {
    for (int k = 0; k < numIntVariables; k++) {
        if (strcmp(intVariables[k].name, name) == 0) {
            return intVariables[k].value;
        }
    }
    return -1;
}
char* findVariableType(char* name) {
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, name) == 0) {
            return "text";
        }
    }
    for (int k = 0; k < numIntVariables; k++) {
        if (strcmp(intVariables[k].name, name) == 0) {
            return "int";
        }
    }
    return "";
}


void newVariable(char* type, char* name) {
    int variable_name_length= strlen(name);
    //identifier için size kontrolü
    if(variable_name_length < 30) {
        for (int i=0 ; i< variable_name_length;i++) {
            if (i ==0) {
                //harf ile başlamama hatası için kontrol yapılıyor
                if (!((name[i] >= 65 && name [i] <=90) || (name[i] >= 97 && name[i] <= 122))){
                    return;
                }
            }
                //digit letter underscore harici hata kontrolü yapılıyor
            else if((name[i]>=33 && name[i]<= 47) ||(name[i]>=58 && name[i]<= 64) || (name[i]>=91 && name[i]<= 94) ||(name[i]>=123 && name[i]<= 126) || name[i] == 96) {
                return;
            }
        }
    }
    else {
        printf("Maximum identifier size is 30 characters !");
        return;
    }
    if (strcmp(type, "text") == 0) {
        strcpy(textVariables[numTextVariables].name, name);
        strcpy(textVariables[numTextVariables].value, "");
        numTextVariables++;
    } else if (strcmp(type, "int") == 0) {
        strcpy(intVariables[numIntVariables].name, name);
        intVariables[numIntVariables].value = 0;
        numIntVariables++;
    } else {
        printf("Error: Invalid variable type '%s'\n", type);
        exit(1);
    }
}

void assignValue(char* variableName, char* value) {
    bool isTextVariable = false;
    bool isIntVariable = false;
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, variableName) == 0) {
            strcpy(textVariables[i].value, value);
            isTextVariable = true;
            break;
        }
    }
    if (!isTextVariable) {
        if (asText(value) >=0) {
            for (int i = 0; i < numIntVariables; i++) {
                if (strcmp(intVariables[i].name, variableName) == 0) {
                    intVariables[i].value = asText(value);
                    isIntVariable = true;
                    break;
                }
            }
        }
        else {
            printf("Negatif deger atanamaz!");
            return;
        }
    }

    if (!isTextVariable && !isIntVariable) {
        printf("Error: Variable '%s' not found\n", variableName);
        exit(1);
    }
}
void performOperationDigital(char* variableName, char* operation, char* operand1, int value2) {
    int value1;
    for (int i = 0; i < numIntVariables; i++) {
        if (strcmp(intVariables[i].name, operand1) == 0) {
            value1 = intVariables[i].value;
            break;
        }
    }
    if (strcmp(operation, "+") == 0) {
        for (int i = 0; i < numIntVariables; i++) {
            if (strcmp(intVariables[i].name, variableName) == 0) {
                intVariables[i].value = value1 + value2;
                return;
            }
        }
    } else if (strcmp(operation, "-") == 0) {
        for (int i = 0; i < numIntVariables; i++) {
            if (strcmp(intVariables[i].name, variableName) == 0) {
                if (value2 > value1) {
                    printf("Error: Subtracting a larger value from a smaller value\n");
                    exit(1);
                }
                intVariables[i].value = value1 - value2;
                return;
            }
        }
    } else {
        printf("Error: Invalid operation '%s'\n", operation);
        exit(1);
    }
}

void performOperationIntVariable(char* variableName, char* operation, char* operand1, char* operand2) {
    int value1, value2;
    for (int i = 0; i < numIntVariables; i++) {
        if (strcmp(intVariables[i].name, operand1) == 0) {
            value1 = intVariables[i].value;
            break;
        }
    }
    for (int i = 0; i < numIntVariables; i++) {
        if (strcmp(intVariables[i].name, operand2) == 0) {
            value2 = intVariables[i].value;
            break;
        }
    }
    if (strcmp(operation, "+") == 0) {
        for (int i = 0; i < numIntVariables; i++) {
            if (strcmp(intVariables[i].name, variableName) == 0) {
                intVariables[i].value = value1 + value2;
                return;
            }
        }
    } else if (strcmp(operation, "-") == 0) {
        for (int i = 0; i < numIntVariables; i++) {
            if (strcmp(intVariables[i].name, variableName) == 0) {
                if (value2 > value1) {
                    printf("Error: Subtracting a larger value from a smaller value\n");
                    exit(1);
                }
                intVariables[i].value = value1 - value2;
                return;
            }
        }
    } else {
        printf("Error: Invalid operation '%s'\n", operation);
        exit(1);
    }
}
void performOperationString(char* variableName, char* operation, char* operand1, char* value2) {
    char* value1;
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, operand1) == 0) {
            value1 = textVariables[i].value;
            break;
        }
    }
    if (strcmp(operation, "+") == 0) {
        for (int i = 0; i < numTextVariables; i++) {
            if (strcmp(textVariables[i].name, variableName) == 0) {
                int len1= strlen(value1);
                int len2= strlen(value2);

                strcat(value1," ");
                strcpy(textVariables[i].value,value1);
                strcat(textVariables[i].value,value2);
            }
        }
    } else if (strcmp(operation, "-") == 0) {
        for (int i = 0; i < numTextVariables; i++) {
            if (strcmp(textVariables[i].name, variableName) == 0) {
                if (value2 > value1) {
                    printf("Error: Subtracting a larger value from a smaller value\n");
                    exit(1);
                }
                intVariables[i].value = value1 - value2;
                return;
            }
        }
    } else {
        printf("Error: Invalid operation '%s'\n", operation);
        exit(1);
    }
}
void performOperationTextVariable(char* variableName, char* operation, char* operand1, char* operand2) {
    char* value1;
    char* value2;
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, operand1) == 0) {
            value1 = textVariables[i].value;
            break;
        }
    }
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, operand2) == 0) {
            value2 = textVariables[i].value;
            break;
        }
    }
    if (strcmp(operation, "+") == 0) {
        for (int i = 0; i < numTextVariables; i++) {
            if (strcmp(textVariables[i].name, variableName) == 0) {
                int len1= strlen(value1);
                int len2= strlen(value2);

                strcat(value1," ");
                strcpy(textVariables[i].value,value1);
                strcat(textVariables[i].value,value2);
            }
        }
    } else if (strcmp(operation, "-") == 0) {
        for (int i = 0; i < numTextVariables; i++) {
            if (strcmp(textVariables[i].name, variableName) == 0) {
                if (value2 > value1) {
                    printf("Error: Subtracting a larger value from a smaller value\n");
                    exit(1);
                }
                intVariables[i].value = value1 - value2;
                return;
            }
        }
    } else {
        printf("Error: Invalid operation '%s'\n", operation);
        exit(1);
    }
}

void readFromFile(char* variableName, char* fileName) {
    strcat(fileName,".txt"); // txt uzantılı dosya okuması için adın sonuna uzantı eklendi
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", fileName);
        exit(1);
    }
    char buffer[MAX_STRING_SIZE + 1];
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character
        for (int i = 0; i < numTextVariables; i++) {
            if (strcmp(textVariables[i].name, variableName) == 0) {
                strcpy(textVariables[i].value, buffer);
                fclose(file);
                return;
            }
        }
    }
    fclose(file);
    printf("Error: Failed to read from file '%s'\n", fileName);
    exit(1);
}

void writeToFile(char* variableName, char* fileName) {
    strcat(fileName,".txt"); // txt uzantılı dosya okuması için adın sonuna uzantı eklendi
    FILE* file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", fileName);
        exit(1);
    }
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, variableName) == 0) {
            fputs(textVariables[i].value, file);
            fclose(file);
            return;
        }
    }
    fclose(file);
    printf("Error: Variable '%s' not found\n", variableName);
    exit(1);
}

void takeInput(char* variableName, char* promptText) {
   // printf("%s", promptText);
    char input[MAX_STRING_SIZE + 1];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove newline character
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, variableName) == 0) {
            strcpy(textVariables[i].value, input);
            return;
        }
    }
    printf("Error: Variable '%s' not found\n", variableName);
    exit(1);
}

void outputText(char* variableName) {
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, variableName) == 0) {
            printf("Output: %s \n", textVariables[i].value);
            return;
        }
    }
    for (int i = 0; i < numIntVariables; i++) {
        if (strcmp(intVariables[i].name, variableName) == 0) {
            printf("Output: %d \n", intVariables[i].value);
            return;
        }
    }
    printf("Error: Variable '%s' not found\n", variableName);
    exit(1);
}

char* subsText(char* text, int begin, int end) {
    //Between dendiği için aradaki harfler alındı örneğin 1-3 için sadece 2. indexteki karakter döndürüldü.
    int length = strlen(text);
    if (begin <= 0 || begin > length || end < begin || end > length) {
        printf("Error: Invalid substring range\n");
        exit(1);
    }
    int resultLength = end - begin -1  ;
    char* result = malloc(resultLength + 1);
    strncpy(result, text + begin +1  , resultLength);
    result[resultLength] = '\0';
    return result;
}
char* subs(char* myText,int begin,int end) {
    char* result;
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, myText) == 0) {
            result = subsText(textVariables[i].value, begin, end);
            return result;
        }
    }
}

char* insertText(char* text, int location, char* insertText) {
    int length = strlen(text);
    if (location <= 0 || location > length + 1) {
        printf("Error: Invalid insertion location\n");
        exit(1);
    }
    int insertLength = strlen(insertText);
    int resultLength = length + insertLength;
    char* result = malloc(resultLength + 1);
    strncpy(result, text, location - 1);
    strcpy(result + location - 1, insertText);
    strcpy(result + location - 1 + insertLength, text + location - 1);
    return result;
}
char* overrideText(char* text, int location, char* ovrText) {
    int length = strlen(text);
    int ovrLength = strlen(ovrText);
    if (location <= 0 || location > length || ovrLength == 0) {
        printf("Error: Invalid override location or text\n");
        exit(1);
    }
    char* result = malloc(length + 1);
    if (result == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }
    strcpy(result, text);
    int count = strlen(result + location - 1);
    strncpy(result + location - 1, ovrText, count);
    return result;
}
char* override(char* myText, int location, char* ovrText) {
    char* result;
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, myText) == 0) {
            result = overrideText(textVariables[i].value, location, ovrText);
            return result;
        }
    }
}
char* asString(int number) {
    char* result = malloc(MAX_STRING_SIZE + 1);
    snprintf(result, MAX_STRING_SIZE + 1, "%d", number);
    return result;
}
int asText(char* text) {
    return atoi(text);
}
int size(char* myText) {
    int size = 0;
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, myText) == 0) {
            size = strlen(textVariables[i].value);
            return size;
        }
    }

}
int locate(char* bigText,char* smallText,int start) {
    //pozisyon döndürülüyor index değil
    int position = -1;
    for (int i = 0; i < numTextVariables; i++) {
        if (strcmp(textVariables[i].name, bigText) == 0) {
            position = locateSubstring(textVariables[i].value, smallText, start);
            return  position;
        }
    }
}
int locateSubstring(char* bigText, char* smallText, int start) {
    char* result = strstr(bigText+start, smallText);
    if (result != NULL) {
        return result - bigText + 1;
    } else {
        return 0;
    }
}

int subtractSubstring(char* minuend, const char* subtrahend) {
    char* substr = strstr(minuend, subtrahend);  // Subtrahend'ı Minuend içinde ara
    if (substr != NULL) {
            memmove(substr, substr + strlen(subtrahend), strlen(substr + strlen(subtrahend)) + 1);
            // substr'ın üzerine sonraki karakterleri taşı
        return 1;
    }
    else {
        for (int i = 0; i < numTextVariables; i++) {
            if (strcmp(textVariables[i].value, minuend) == 0) {
                //aranan içerik bulunamadıysa output bos döndürmesi için işlem yapılır
                strcpy(textVariables[i].value, "");
            }
        }
        if (strlen(minuend) < strlen(subtrahend)) {
            return -1;
        }
        else {
            return -2;
        }
    }
}