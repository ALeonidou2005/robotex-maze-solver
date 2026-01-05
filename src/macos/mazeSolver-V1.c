#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ARRASIZE 16+1
#define DoNotGoAgain 99

#define RED  "\x1B[31m"
#define RESET "\x1B[0m"

void KeepSpaceCheck(int num){
    if (num < 10){
        printf(" ");
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

int CheckForAgain(){
    int Again;
    printf("\nEnter 0 or 1 to 'run again or not' here please: ");
    scanf("%d",&Again);
    return Again;
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
            default:
                Orientation = Orientation + 2;
        }
    }
    
    return Orientation;
}

int main(){
    int Num[ARRASIZE][ARRASIZE];
    
    int i, j;
    
    int CurrentX = 1, CurrentY = 16;
    int NextX, NextY;
    
    int AvailablePointsValue[5], AvailablePointsCoordinates[5][3];
    
    int f_INPUT, r_INPUT, l_INPUT, Sum_INPUT;
    
    int Current_Orientation;
    int Next_Orientation, Possible_Right_Orientation, Possible_Left_Orientation, Possible_Uturn_Orientation;
    
    int MainInputCounter = 0;
    
    bool Still100 = false;
    
    //Initialization of Array
    for (i = 1; i <= 8; i++){
        for (j = 1; j<= 8; j++){
            Num[i][j] = 16 - j - i;
            Num[16-i+1][j] = 16 - j - i;
        }
        for (j = 9; j <= 16; j++){
            Num[i][j] = j - i - 1;
            Num[16-i+1][j] = j - i - 1;
        }
    }
    
    //INPUT PROCESS
    system("clear");
    printf("The initial position of the robot is (1, 16).");
    printf("\nEnter initial orientation here please (1, 2, 3, 4): ");
    scanf("%d", &Next_Orientation);
    
    do{          //Main Loop.
    MainInputCounter++;
    NextSectionLine(MainInputCounter);
    printf("Enter INPUT for sensors. Use (0 or 1) => (false or true)...");
    printf("\nINPUT OF 'forward' SENSOR: ");
    scanf("%d", &f_INPUT);
    printf("\nINPUT OF 'right' SENSOR: ");
    scanf("%d", &r_INPUT);
    printf("\nINPUT OF 'left' SENSOR: ");
    scanf("%d", &l_INPUT);

    //Some inside loop initalisations...
    Current_Orientation = Next_Orientation;
    Possible_Right_Orientation = FindPossibleOrientation(Current_Orientation,'r');
    Possible_Left_Orientation  = FindPossibleOrientation(Current_Orientation,'l');
    Possible_Uturn_Orientation = FindPossibleOrientation(Current_Orientation,'u');
    
    //Finding Nearby Points.       (N = 1, E = 2, S = 3, W = 4)
    if(CurrentY - 1 > 0){
        AvailablePointsValue[1] = Num[CurrentY-1][CurrentX];
        AvailablePointsCoordinates[1][2] = CurrentY-1;
    }
    else{
        AvailablePointsValue[1] = DoNotGoAgain;
        AvailablePointsCoordinates[1][2] = -2;
    }
    AvailablePointsCoordinates[1][1] = CurrentX;
    
    if(CurrentX + 1 <= 16){
        AvailablePointsValue[2] = Num[CurrentY][CurrentX+1];
        AvailablePointsCoordinates[2][1] = CurrentX+1;
    }
    else{
        AvailablePointsValue[2] = DoNotGoAgain;
        AvailablePointsCoordinates[2][1] = -2;
    }
    AvailablePointsCoordinates[2][2] = CurrentY;

    if(CurrentY + 1 <= 16){
        AvailablePointsValue[3] = Num[CurrentY+1][CurrentX];
        AvailablePointsCoordinates[3][2] = CurrentY+1;
    }
    else{
        AvailablePointsValue[3] = DoNotGoAgain;
        AvailablePointsCoordinates[3][2] = -2;
    }
    AvailablePointsCoordinates[3][1] = CurrentX;
        
    if(CurrentX - 1 > 0){
        AvailablePointsValue[4] = Num[CurrentY][CurrentX-1];
        AvailablePointsCoordinates[4][1] = CurrentX-1;
    }
    else{
        AvailablePointsValue[4] = DoNotGoAgain;
        AvailablePointsCoordinates[4][1] = -2;
    }
    AvailablePointsCoordinates[4][2] = CurrentY;
    
    //Considering the 100 value/DO NOT GO AGAIN places.
    if (AvailablePointsValue[Next_Orientation] == DoNotGoAgain)
        f_INPUT = 0;
    if (AvailablePointsValue[Possible_Right_Orientation] == DoNotGoAgain)
        r_INPUT = 0;
    if (AvailablePointsValue[Possible_Left_Orientation] == DoNotGoAgain)
        l_INPUT = 0;
        
    //CHOOSING WHICH LOGIC TO USE.
    Sum_INPUT = f_INPUT+r_INPUT+l_INPUT;
        
    system("clear");
    if (Sum_INPUT == 0){
        printf("Turn RIGHT 180 degrees. Then GO FORWARD. ALSO mark as 100.");
        Still100 = true;
        Next_Orientation = Possible_Uturn_Orientation;
        NextX = AvailablePointsCoordinates[Next_Orientation][1];
        NextY = AvailablePointsCoordinates[Next_Orientation][2];
    }
    else
    if (Sum_INPUT == 1){
        if (f_INPUT == 1){
            printf("GO FORWARD");
        }
        if (r_INPUT == 1){
            printf("Turn RIGHT 90 degrees. Then GO FORWARD");
            Next_Orientation = Possible_Right_Orientation;
        }
        if (l_INPUT == 1){
            printf("Turn RIGHT 90 degrees. Then GO FORWARD");
            Next_Orientation = Possible_Left_Orientation;
        }
        NextX = AvailablePointsCoordinates[Next_Orientation][1];
        NextY = AvailablePointsCoordinates[Next_Orientation][2];
    }
    else
    if (Sum_INPUT == 2){
        if (f_INPUT == 1 && r_INPUT == 1){
            printf("CHOOSE BETWEEN FORWARD AND RIGHT");
            if(AvailablePointsValue[Next_Orientation]<=AvailablePointsValue[Possible_Right_Orientation])
            {
                printf("\t=> GO FORWARD");
            }
            else
            {
                printf("\t=> Turn RIGHT 90 degrees. Then GO FORWARD");
                Next_Orientation = Possible_Right_Orientation;
            }
        }
        if (f_INPUT == 1 && l_INPUT == 1){
            printf("CHOOSE BETWEEN FORWARD AND LEFT");
            if(AvailablePointsValue[Next_Orientation]<=AvailablePointsValue[Possible_Left_Orientation])
            {
                printf("\t=> GO FORWARD");
            }
            else
            {
                printf("\t=> Turn LEFT 90 degrees. Then GO FORWARD");
                Next_Orientation = Possible_Left_Orientation;
            }
        }
        if (r_INPUT == 1 && l_INPUT == 1){
            printf("CHOOSE BETWEEN RIGHT AND LEFT");
        if(AvailablePointsValue[Possible_Right_Orientation]<=AvailablePointsValue[Possible_Left_Orientation])
            {
                printf("\t=> Turn RIGHT 90 degrees. Then GO FORWARD");
                Next_Orientation = Possible_Right_Orientation;
            }
            else
            {
                printf("\t=> Turn LEFT 90 degrees. Then GO FORWARD");
                Next_Orientation = Possible_Left_Orientation;
            }
        }
        NextX = AvailablePointsCoordinates[Next_Orientation][1];
        NextY = AvailablePointsCoordinates[Next_Orientation][2];
        Still100 = false;
    }
    else
    if (Sum_INPUT == 3){
        printf("CHOOSE BETWEEN ALL 3 OF THEM");
        if(AvailablePointsValue[Next_Orientation]<=AvailablePointsValue[Possible_Right_Orientation] && AvailablePointsValue[Next_Orientation] <= AvailablePointsValue[Possible_Left_Orientation]){
            printf("\t=> GO FORWARD");
        }
        else if(AvailablePointsValue[Possible_Right_Orientation]<=AvailablePointsValue[Possible_Left_Orientation])
            {
                printf("\t=> Turn RIGHT 90 degrees. Then GO FORWARD");
                Next_Orientation = Possible_Right_Orientation;
            }
        else{
            printf("\t=> Turn LEFT 90 degrees. Then GO FORWARD");
            Next_Orientation = Possible_Left_Orientation;
        }
        NextX = AvailablePointsCoordinates[Next_Orientation][1];
        NextY = AvailablePointsCoordinates[Next_Orientation][2];
        Still100 = false;
    }
    
    printf("\n • Current Coordinates: (%d, %d)",CurrentX,CurrentY);
    printf("\n • Next Coordinates: (%d, %d)",NextX,NextY);
    printf("\n • Current Orientation: %d",Current_Orientation);
    printf("\n • Next Orientation: %d\n",Next_Orientation);
        
        if (Still100 == true)
            Num[CurrentY][CurrentX] = DoNotGoAgain;
        else
            Num[CurrentY][CurrentX] = Num[CurrentY][CurrentX] + 3;
        CurrentX = NextX;
        CurrentY = NextY;

        //OUTPUT
        printf("\t\t\t%sRESULTS%s",RED,RESET);      //Using some coloured Text...
        printf("\n   ");
        for (i = 1; i <= 16; i++){
            for (j = 1; j <= 16; j++){
                printf("%d ",Num[i][j]);
                KeepSpaceCheck(Num[i][j]);
            }
            printf("\n   ");
        }
    }while(CheckForAgain() == 1);
    
    printf("\n\tProgram Terminated!! Hope it worked and was useful...\n\n");
    return 0;
}
