#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ARRASIZE 16+1
#define BypassARRAYSIZE 35+1
#define DoNotGoAgain 99
#define SubtractVal 15

#define RED  "\x1B[31m"
#define RESET "\x1B[0m"
#define BOLD "\x1B[1m"

void KeepSpaceCheck(int num){   //Simple formatting check
    if (num>=0){
        if (num < 10){
            printf(" ");
        }
    }
}

void NextSectionLine(int HowManyTimes){
    printf("\n");
    system("clear");
    system("clear");
    printf("*-----------------------------------------------------------------*\n");
    printf("\t\t\t\t%d\n",HowManyTimes);
    printf("*-----------------------------------------------------------------*\n\n");
}

int CheckForAgain(int p1, int p2, int p3, int p4){
    int TotalZeros = 0;
    int buttonInput;
    
    //Showing the "CENTER LOGIC"
    if (p1 == 0)
        TotalZeros++;
    if (p2 == 0)
        TotalZeros++;
    if (p3 == 0)
        TotalZeros++;
    if (p4 == 0)
        TotalZeros++;
    
    if (TotalZeros != 2){
        printf("\nHave you pressed the button? (0 or 1): ");
        scanf("%d",&buttonInput);
        if (buttonInput == 1){
            TotalZeros = 3;        //Showing the "FORCE LOGIC"
        }
    }
    return TotalZeros;
}

int FindPossibleOrientation(int Orientation, char TypeOfTurn){
    if (TypeOfTurn == 'r'){
        if (Orientation == 4){
            Orientation = 1;
        }
        else{
            Orientation++;
        }
    }
    else if (TypeOfTurn == 'l'){
        if (Orientation == 1){
            Orientation = 4;
        }
        else{
            Orientation--;
        }
    }
    else if (TypeOfTurn == 'u'){
        switch (Orientation){
            case 3:
                Orientation = 1;
                break;
            case 4:
                Orientation = 2;
                break;
            default:
                Orientation = Orientation + 2;
                break;
        }
    }
    
    return Orientation;
}

int AskForBypassInput(int DifVar, char SensorType){
    int BOOLans;
    if (SensorType == 'l')
        printf("\nDo you have space for %d square(s) from the left sensor?... ",DifVar);
    if (SensorType == 'r')
        printf("\nDo you have space for %d square(s) from the right sensor?... ",DifVar);
    scanf("%d",&BOOLans);
    
    return BOOLans;
}

int main(){
    int Num[ARRASIZE][ARRASIZE], CleanNum[ARRASIZE][ARRASIZE], DifNum[ARRASIZE][ARRASIZE];
    
    int IndexBypassNum[BypassARRAYSIZE][2], IndexBypassOrientation[BypassARRAYSIZE];
    int BypassDifVar;
    
    int i, j;
    
    int CurrentX, CurrentY;
    int NextX, NextY;
    
    int AvailablePointsValue[5], AvailablePointsCoordinates[5][2];
    
    int f_INPUT, r_INPUT, l_INPUT, Sum_INPUT;
    
    int Current_Orientation;
    int Next_Orientation, Possible_Right_Orientation, Possible_Left_Orientation, Possible_Uturn_Orientation;
    
    int LogicMainLoopCounter, ProgramMainLoopCounter = 0;
    int MainProgramAgain;
    int MainLogicLoopTermination;
    
    int LastTurnX, LastTurnY;
    
    bool DoNotIndexBypass;
    bool Still100 = false;
    
    int TotalTurns = 0;
    int TotalMoves = 0;
    int LiftDelay = 0;
    
    //Initialization of the 2 2D-arrays.
    for (i = 1; i <= 8; i++){
        for (j = 1; j<= 8; j++){
            Num[i][j] = 16 - j - i;
            Num[16-i+1][j] = 16 - j - i;
            CleanNum[i][j] = 16 - j - i;
            CleanNum[16-i+1][j] = 16 - j - i;
        }
        for (j = 9; j <= 16; j++){
            Num[i][j] = j - i - 1;
            Num[16-i+1][j] = j - i - 1;
            CleanNum[i][j] = j - i - 1;
            CleanNum[16-i+1][j] = j - i - 1;
        }
    }
    
    do{     //Program Main Loop.
    //Some Main Loop Initializations
    ProgramMainLoopCounter++;
    DoNotIndexBypass = false;
    LogicMainLoopCounter = 0;
    if (MainLogicLoopTermination == 3){     //Checking if it was forced to terminate or not.
        Num[LastTurnY][LastTurnX] = DoNotGoAgain;
    }
    CurrentX = 1;
    CurrentY = 16;
    
    system("clear");
    printf("\t%sMAZE ROUND %d%s\n",BOLD,ProgramMainLoopCounter,RESET);
    printf("Enter the initial position X: ");
    scanf("%d",&CurrentX);
    printf("\nEnter the initial position Y: ");
    scanf("%d",&CurrentY);
    printf("\nEnter initial orientation (1, 2, 3, 4): ");
    scanf("%d", &Next_Orientation);
    
    if (ProgramMainLoopCounter >= 2)        //Initializing the Dif Array!
        for (i = 1; i <= 16; i++)
            for(j = 1; j <= 16; j++){
                DifNum[i][j] = Num[i][j] - CleanNum[i][j];
                if (DifNum[i][j] == 3)     //Checking ONCE tracked...
                    Num[i][j] = CleanNum[i][j] - SubtractVal;
                else if (DifNum[i][j] > 3)       //Checking MULTIPLE tracked...
                    if (Num[i][j] != DoNotGoAgain && Num[i][j] != 0)
                        if (Num[i+1][j] == DoNotGoAgain || Num[i-1][j] == DoNotGoAgain || Num[i][j+1] == DoNotGoAgain || Num[i][j-1] == DoNotGoAgain){
                            Num[i][j] = CleanNum[i][j] - SubtractVal;
                        }
                }
    //Initialisation of bypass arrays   (cleaned)
    for (i = 1; i<=BypassARRAYSIZE-1; i++){
        IndexBypassNum[i][0] = 0;
        IndexBypassNum[i][1] = 0;
        IndexBypassOrientation[i] = 0;
    }

    do{          //Logic Main Loop.
    //Some logic loop initalizations...
    LogicMainLoopCounter++;
    Current_Orientation = Next_Orientation;
    Possible_Right_Orientation = FindPossibleOrientation(Current_Orientation,'r');
    Possible_Left_Orientation  = FindPossibleOrientation(Current_Orientation,'l');
    Possible_Uturn_Orientation = FindPossibleOrientation(Current_Orientation,'u');
      
    //Finding Nearby Points.       (N = 1, E = 2, S = 3, W = 4)
    if(CurrentY - 1 > 0){
        AvailablePointsValue[1] = Num[CurrentY-1][CurrentX];
        AvailablePointsCoordinates[1][1] = CurrentY-1;
    }
    else{
        AvailablePointsValue[1] = DoNotGoAgain;
    }
    AvailablePointsCoordinates[1][0] = CurrentX;
    
    if(CurrentX + 1 <= 16){
        AvailablePointsValue[2] = Num[CurrentY][CurrentX+1];
        AvailablePointsCoordinates[2][0] = CurrentX+1;
    }
    else{
        AvailablePointsValue[2] = DoNotGoAgain;
    }
    AvailablePointsCoordinates[2][1] = CurrentY;
        
    if(CurrentY + 1 <= 16){
        AvailablePointsValue[3] = Num[CurrentY+1][CurrentX];
        AvailablePointsCoordinates[3][1] = CurrentY+1;
    }
    else{
        AvailablePointsValue[3] = DoNotGoAgain;
    }
    AvailablePointsCoordinates[3][0] = CurrentX;
        
    if(CurrentX - 1 > 0){
        AvailablePointsValue[4] = Num[CurrentY][CurrentX-1];
        AvailablePointsCoordinates[4][0] = CurrentX-1;
    }
    else{
        AvailablePointsValue[4] = DoNotGoAgain;
    }
    AvailablePointsCoordinates[4][1] = CurrentY;
        
    //INPUT PROCESS
    NextSectionLine(LogicMainLoopCounter);
    printf("%sNOTE:%s Current Coordinates after last movement are: %s(%d,%d)%s\n",BOLD,RESET,BOLD,CurrentX,CurrentY,RESET);
    printf("\nEnter INPUT for sensors. Use (0 or 1) => (false or true)...");
    printf("\nINPUT OF 'forward' SENSOR: ");
    scanf("%d", &f_INPUT);
    printf("\nINPUT OF 'right' SENSOR: ");
    scanf("%d", &r_INPUT);
    printf("\nINPUT OF 'left' SENSOR: ");
    scanf("%d", &l_INPUT);

    //Considering the 100 value/DO NOT GO AGAIN places.
    if (AvailablePointsValue[Next_Orientation] == DoNotGoAgain){
        f_INPUT = 0;
    }
    if (AvailablePointsValue[Possible_Right_Orientation] == DoNotGoAgain){
        r_INPUT = 0;
    }
    if (AvailablePointsValue[Possible_Left_Orientation] == DoNotGoAgain){
        l_INPUT = 0;
    }
    Sum_INPUT = f_INPUT+r_INPUT+l_INPUT;    //Setting Sum of INput to use later...
        
    //Bypass logic "10"...
    for (i = 1; i <= BypassARRAYSIZE-1; i++){     //searching in arrays
        if ((IndexBypassOrientation[i] == Next_Orientation) && (NextX == IndexBypassNum[i][0] || NextY == IndexBypassNum[i][1]) && (Sum_INPUT == 2 || Sum_INPUT == 3)){

            if (NextY == IndexBypassNum[i][1])              //Starting Bypass Logic "11"!!
                BypassDifVar = NextX - IndexBypassNum[i][0];
            else if (NextX == IndexBypassNum[i][0])
                BypassDifVar = NextY - IndexBypassNum[i][1];
            
            if (Num[NextY][NextX] - CleanNum[NextY][NextX] != 0){
                DoNotIndexBypass = true;
            }
            else{
                printf("%s\nLOGIC 11 ACTIVATED: Orient = %d%s",RED,Next_Orientation,RESET);
                printf("\nStarting Bypass Input Logic:");
                if (BypassDifVar > 0){
                    printf("\nPositive...");
                    switch (Next_Orientation){
                        case 1:
                            printf("C1\n");
                            if (AskForBypassInput(BypassDifVar,'l') == 1){
                                Num[IndexBypassNum[i][1]+1][IndexBypassNum[i][0]] = DoNotGoAgain;
                                Num[NextY+1][NextX] = DoNotGoAgain;
                            }
                            break;
                        case 2:
                            printf("C2\n");
                            if (AskForBypassInput(BypassDifVar,'l') == 1){
                                Num[IndexBypassNum[i][1]][IndexBypassNum[i][0]-1] = DoNotGoAgain;
                                Num[NextY][NextX-1] = DoNotGoAgain;
                            }
                            break;
                        case 3:
                            printf("C3\n");
                            if (AskForBypassInput(BypassDifVar,'r') == 1){
                                Num[IndexBypassNum[i][1]-1][IndexBypassNum[i][0]] = DoNotGoAgain;
                                Num[NextY-1][NextX] = DoNotGoAgain;
                            }
                            break;
                        case 4:
                            printf("C4\n");
                            if (AskForBypassInput(BypassDifVar,'r') == 1){
                                Num[IndexBypassNum[i][1]][IndexBypassNum[i][0]+1] = DoNotGoAgain;
                                Num[NextY][NextX+1] = DoNotGoAgain;
                            }
                            break;
                    }
                }
                else if (BypassDifVar < 0){
                    printf("\nNegative...");
                    switch (Next_Orientation){
                        case 1:
                            printf("C1\n");
                            if (AskForBypassInput(BypassDifVar * -1,'r') == 1){
                                Num[IndexBypassNum[i][1]+1][IndexBypassNum[i][0]] = DoNotGoAgain;
                                Num[NextY+1][NextX] = DoNotGoAgain;
                            }
                            break;
                        case 2:
                            printf("C2\n");
                            if (AskForBypassInput(BypassDifVar * -1,'r') == 1){
                                Num[IndexBypassNum[i][1]][IndexBypassNum[i][0]-1] = DoNotGoAgain;
                                Num[NextY][NextX-1] = DoNotGoAgain;
                            }
                            break;
                        case 3:
                            printf("C3\n");
                            if (AskForBypassInput(BypassDifVar * -1,'l') == 1){
                                Num[IndexBypassNum[i][1]-1][IndexBypassNum[i][0]] = DoNotGoAgain;
                                Num[NextY-1][NextX] = DoNotGoAgain;
                            }
                            break;
                        case 4:
                            printf("C4\n");
                            if (AskForBypassInput(BypassDifVar * -1,'l') == 1){
                                Num[IndexBypassNum[i][1]][IndexBypassNum[i][0]+1] = DoNotGoAgain;
                                Num[NextY][NextX+1] = DoNotGoAgain;
                            }
                            break;
                    }
                }
            }
        }
    }   //INPUT PROCESS COMPLETELY DONE
        
    //CHOOSING WHICH LOGIC TO USE...
    system("clear");
    if (Sum_INPUT == 0){
        printf("Turn RIGHT 180 degrees. Then GO FORWARD. ALSO mark cell as NotGoAgain.");
        TotalTurns = TotalTurns + 2;
        Still100 = true;
        Next_Orientation = Possible_Uturn_Orientation;
    }
    else
    if (Sum_INPUT == 1){
        if (f_INPUT == 1){
            printf("GO FORWARD");
        }
        if (r_INPUT == 1){
            printf("Turn RIGHT 90 degrees. Then GO FORWARD");
            TotalTurns++;
            Next_Orientation = Possible_Right_Orientation;
        }
        if (l_INPUT == 1){
            printf("Turn LEFT 90 degrees. Then GO FORWARD");
            TotalTurns++;
            Next_Orientation = Possible_Left_Orientation;
        }
    }
    else
    if (Sum_INPUT == 2){
        if (f_INPUT == 1 && r_INPUT == 1){
            printf("CHOOSE BETWEEN FORWARD AND RIGHT");
            if(AvailablePointsValue[Next_Orientation]<=AvailablePointsValue[Possible_Right_Orientation])
            {
                printf("   => GO FORWARD");
            }
            else
            {
                printf("   => Turn RIGHT 90 degrees. Then GO FORWARD");
                TotalTurns++;
                Next_Orientation = Possible_Right_Orientation;
            }
        }
        if (f_INPUT == 1 && l_INPUT == 1){
            printf("CHOOSE BETWEEN FORWARD AND LEFT");
            if(AvailablePointsValue[Next_Orientation]<=AvailablePointsValue[Possible_Left_Orientation])
            {
                printf("   => GO FORWARD");
            }
            else
            {
                printf("   => Turn LEFT 90 degrees. Then GO FORWARD");
                TotalTurns++;
                Next_Orientation = Possible_Left_Orientation;
            }
        }
        if (r_INPUT == 1 && l_INPUT == 1){
            printf("CHOOSE BETWEEN RIGHT AND LEFT");
        if(AvailablePointsValue[Possible_Right_Orientation]<=AvailablePointsValue[Possible_Left_Orientation])
            {
                printf("   => Turn RIGHT 90 degrees. Then GO FORWARD");
                TotalTurns++;
                Next_Orientation = Possible_Right_Orientation;
            }
            else
            {
                printf("   => Turn LEFT 90 degrees. Then GO FORWARD");
                TotalTurns++;
                Next_Orientation = Possible_Left_Orientation;
            }
        }
    }
    else
    if (Sum_INPUT == 3){
        printf("CHOOSE BETWEEN ALL 3 OF THEM");
        if(AvailablePointsValue[Next_Orientation]<=AvailablePointsValue[Possible_Right_Orientation] && AvailablePointsValue[Next_Orientation] <= AvailablePointsValue[Possible_Left_Orientation]){
            printf("   => GO FORWARD");
        }
        else if(AvailablePointsValue[Possible_Right_Orientation]<=AvailablePointsValue[Possible_Left_Orientation])
            {
                printf("   => Turn RIGHT 90 degrees. Then GO FORWARD");
                TotalTurns++;
                Next_Orientation = Possible_Right_Orientation;
            }
        else{
            printf("   => Turn LEFT 90 degrees. Then GO FORWARD");
            TotalTurns++;
            Next_Orientation = Possible_Left_Orientation;
        }
    }
    else{   //Not adding when round is illegible!
        TotalMoves--;
        LogicMainLoopCounter--;
        printf("%sIllegible round / Invalid Input  =>  ROUND IGNORED%s",RED,RESET);
    }
        
    if (Sum_INPUT<4){       //This code will only run when the round is legible!
        NextX = AvailablePointsCoordinates[Next_Orientation][0];
        NextY = AvailablePointsCoordinates[Next_Orientation][1];
        if (Sum_INPUT == 2 || Sum_INPUT == 3){       //Common 2 & 3 case code...
            LastTurnX = NextX;
            LastTurnY = NextY;
            
            Still100 = false;
            if (DoNotIndexBypass == false){
                i = 0;
                do{     //Searching for available place in the array.
                    i++;
                }while(IndexBypassOrientation[i] != 0);
                IndexBypassNum[i][0] = CurrentX;
                IndexBypassNum[i][1] = CurrentY;
                IndexBypassOrientation[i] = Possible_Uturn_Orientation;
                printf("\n%sBYPASS INDEX STORED IN %d%s",RED,i,RESET);
            }
            else
                DoNotIndexBypass = false;
        }
        if (Still100 == true)
            Num[CurrentY][CurrentX] = DoNotGoAgain;
        else
            Num[CurrentY][CurrentX] = Num[CurrentY][CurrentX] + 3;
    }
    
    printf("\n • Current Coordinates: (%d, %d)",CurrentX,CurrentY);
    printf("\n • Next Coordinates: (%d, %d)",NextX,NextY);
    printf("\n • Current Orientation: %d",Current_Orientation);
    printf("\n • Next Orientation: %d\n",Next_Orientation);
        
    CurrentX = NextX;
    CurrentY = NextY;

        //OUTPUT
        printf("\t\t\t%sRESULTS%s",BOLD,RESET);      //Using some coloured Text...
        printf("\n   ");
        for (i = 1; i <= 16; i++){
            printf("%s%s\n%d   %s",RED,BOLD,i,RESET);
            KeepSpaceCheck(i);
            for (j = 1; j <= 16; j++){
                printf("%d ",Num[i][j]);
                KeepSpaceCheck(Num[i][j]);
            }
        }
        printf("\n\n     ");
        for(i = 1; i<=16; i++){
            printf("%s%s%d %s",RED,BOLD,i,RESET);
            KeepSpaceCheck(i);
        }
        
        printf("\n");
        for (i = 1; i<=20; i++)
            printf("\n%d. (%d,%d) == Orient: %d",i,IndexBypassNum[i][0],IndexBypassNum[i][1],IndexBypassOrientation[i]);
        printf("\nSumInput = %d\n",Sum_INPUT);
        
        MainLogicLoopTermination = CheckForAgain(AvailablePointsValue[1],AvailablePointsValue[2],AvailablePointsValue[3],AvailablePointsValue[4]);
        }while(MainLogicLoopTermination < 2 );
        TotalMoves = TotalMoves + LogicMainLoopCounter;
        printf("\nTo go for the next try press again the 'button'! (0 or 1): ");
        scanf("%d",&MainProgramAgain);
    }while(MainProgramAgain==1);
    
    LiftDelay = (ProgramMainLoopCounter - 1) * 5;
    printf("\n%sWhole Maze Stats: %s",BOLD,RESET);
    printf("\n • Total Moves: %d",TotalMoves-1);
    printf("\n • Total Turns: %d",TotalTurns);
    printf("\n • Lift & replace delay: %ds\n",LiftDelay);
    
    printf("\n\t%sProgram Terminated!! Hope it worked and was useful...\n%s",BOLD,RESET);
    printf("Credit: Made by ANDREAS LEONIDOU in June 2020 | Program Version: 1.11.4.2\n\n");
    return 0;
}
