/*******************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 2
 * Name: Clément VALOT
 * Student ID: 13697394
 * Date of submission: 06/09/2019
 * A brief statement on what you could achieve (less than 50 words):
 * I was able to create an interactive menu where the user can choose what he 
 * wants to do with the program. I learnt to play with structure and I  
 * understood better the file processing with how to use it and why.
 *
 * A brief statement on what you could NOT achieve (less than 50 words):
 * I couldn't achieve the all the verification and tests on invalid inputs
 * entered by the user. I wasn't able to use pointers because their functionning
 * is still new for me and they are kind of tricky so i rathered stick to what
 * I know.
*******************************************************************************/

/*******************************************************************************
 * List header files.
*******************************************************************************/
#include <stdio.h> /*printf, scanf, fprinf, fscanf, fgetc */
#include <string.h> /*strcmp, strcpy, strlen*/

/*******************************************************************************
 * List preprocessing directives.
*******************************************************************************/
#define MAX_FLIGHTCODE_LEN 6
#define MAX_CITYCODE_LEN 3
#define MAX_NUM_FLIGHTS 5
#define MAX_NBR_CHARACTER 100
#define DB_NAME "database"

/*I define here a static integer corresponding to the number of flights the
user enters. I initialize it at 0 as when the program starts, the user hasn't 
enter any flight information yet.*/
static int numberflight=0;

/*******************************************************************************
 * List structs.
*******************************************************************************/
struct date_time
{
    int month;
    int day;
    int hour;
    int minute;
};
typedef struct date_time date_time_t;

struct flight
{
    /*I initialized both flightcode and city code length to 100 to deal with 
    the extra-long input issue*/
    char flightcode [MAX_NBR_CHARACTER];
    date_time_t departure_dt;
    char arrival_city [MAX_NBR_CHARACTER];
    date_time_t arrival_dt;
};
typedef struct flight flight_t;

/*******************************************************************************
 * Function prototypes.
*******************************************************************************/
void print_menu (void);
flight_t Add_Flight(void);
void Display_Flights (flight_t List_flights[],int MAX_NUM_FLIGHTS);
void Print_flight(flight_t flight);
void SaveDatabase(flight_t List_flights[],int MAX_NUM_FLIGHTS);
void LoadDatabase(flight_t List_flights[],int MAX_NUM_FLIGHTS);
int CheckDate(int month, int date, int hour, int minute);
int CheckFlightCode( char* code);

/*******************************************************************************
 * Main
 * I only put the function print_menu because it does everything the user needs
*******************************************************************************/
int main(void)
{
    print_menu();
    return 0;
}

/*******************************************************************************
 * This function prints the initial menu with all instructions on how to use
 * this program.
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void print_menu (void)
{
	/*I start by creating an array of flights with length MAX_NUM_FLIGHTS
	which I will fill later in the program as I add flights.*/
    flight_t List_flights[MAX_NUM_FLIGHTS];
    /*I initialize 3 integers:
    - choice which is the integer the user
    - n which is the integer allowing us to print the message error if the user
    entered a wrong integer for choice
    - exit which allows to continue the program indefinitely until the user
    type '5'. */
    int choice, exit=0, n;
    while (exit==0)
    {
    	/*Each time we start this while loop again, we set the value of n to 0
    	so that it doesn't display the message 'Invalid choice' directly while
    	the user didn't even enter a value yet. However, if he enters a wrong
    	choice, n would have been incremented before and therefore not be equals
    	to 0 anymore what would display the message.*/
        n=0;
        /*I chose here to put a do-while loop for one specific reason : we have
        to do the action (asking the user to enter a value for choice) before
        checking the condition which is the integer entered by the user must be
        either 1, 2, 3, 4 or 5. If not, we go back to the beginning of the do
        loop, displaying the error message.*/
        do
    	{
    	    if(n!=0)
    		{
    		    printf("Invalid choice\n");
    		}
    	    printf("\n"
    	    "1. add a flight\n"
    	    "2. display all flights to a destination\n"
    	    "3. save the flights to the database file\n"
    	    "4. load the flights from the database file\n"
    	    "5. exit the program\n"
    	    "Enter choice (number between 1-5)>\n");
    	    scanf("%d", &choice);
    	    n++;
    	}
        while(choice!=5 && choice!=4 && choice!=3 && choice!=2 && choice!=1);
    	/*Here I put a switch case which I found much more practical and easier
    	to use than if-else conditions.*/
        switch(choice)
    	{
    	    case 1:
    		{
    			/*I placed and if else condition here in case the user wants to
    			add more flights in the array of flights than it is allowed to
    			stock. In order to do that, I defined a constant integer at the
    			beginning of the program (const int numberflight) which I
    			incremented each time I used the function Add_Flight().
    			It also revealed itself useful to fill the array of flights;
    			indeed, since my function Add_Flight() returns a flight, I have
    			to equalize it with another flight : List_flights[numberflight].
    			So the next time the function is called, it returns a flights in
    			the next case of the array because I incremented numberflight
    			the last time the function was called.*/
    		    if(numberflight<MAX_NUM_FLIGHTS)
                {
                    List_flights[numberflight]=Add_Flight();
                    numberflight++;
                }
                else
                {
                	/*If numberflight isn't less than MAX_NUM_FLIGHTS, it means
                	there is no more space in the flights array so we display
                	this message.*/
                    printf("Cannot add more flights - memory full\n");
                }
    		    break;
    		}
    	    case 2:
    		{
    		    Display_Flights(List_flights, MAX_NUM_FLIGHTS);
    		    break;

    		}
    	    case 3:
    		{
    		    SaveDatabase(List_flights, MAX_NUM_FLIGHTS);
    		    break;
    		}
    	    case 4:
    		{
    		    LoadDatabase(List_flights, MAX_NUM_FLIGHTS);
    		    break;
    		}
    	    case 5:
    		{
    			/*This is the core of our first while loop. Like we saw before,
    			the first while loop continue to compile indefinitely while
    			exit=0. So if the user wants to end the program, he entered 5
    			exit takes the value 1 and the while loop is finally broken
    			which ends the program.*/
    		    exit=1;
    		    break;
    		}
    	}
    }
}

/*This function is the basis of our program : it asks the user to enter
information on a flight:
-the flightcode : a string made of two uppercases character from A to Z and 1 to
 4 integers from 1 to 9
-the date of departure : month, day, hour, minute
-the citycode : a string made of 1 to 3 characters (anyone)
-the date of arrival : month, day, hour, minute.
The challenge of this function was to check if the user enters valid information
like a month between 1 and 12 or a good flightcode.*/
flight_t Add_Flight()
{
	/*I start by creating the flight I will return at the end of my function.*/
    flight_t flight;
	/*This integer repeat is going to be present all the way down to the end of the	function because like the integer n in the menu, it is useful to printing	the error messages "Invalid Input". So each time we get out of the do-while loop, I reassign the value 0 to repeat.*/
    int repeat=0;
	/*Again I use do-while loop to do the action first and then check the
	condition*/
    do
	{
	    if(repeat!=0)
		{
		    printf("Invalid input\n");
		}
	    printf("Enter flight code>\n");
	    scanf("%s", flight.flightcode);
	    repeat++;
	}
	/*I explain later the functionning of this function.*/
    while(CheckFlightCode(flight.flightcode)==1);
    repeat=0;
	/*I ask only once this so that I don't put this message again in case of
	wrong entry by the user.*/
    printf("Enter departure info for the flight leaving SYD.\n");
    do
	{
	    if(repeat!=0)
		{
		    printf("Invalid input\n");
		}
	    printf("Enter month, date, hour and minute separated by spaces>\n");
	    scanf("%d %d %d %d", &flight.departure_dt.month,
	    &flight.departure_dt.day, &flight.departure_dt.hour,
	    &flight.departure_dt.minute);
	    repeat++;
	}
	/*I explain later the functionning of this function.*/
    while(CheckDate(flight.departure_dt.month,flight.departure_dt.day,
    flight.departure_dt.hour, flight.departure_dt.minute)==1);
	/*Here, it was a bit tricky to take the value entered by the user and in
	case the string was	more than 3 characters, only keeping the 3 first
	characters.*/
    printf("Enter arrival city code>\n");
	/*So first, I ask the user the city_code which I define as a string.*/
    scanf("%s", flight.arrival_city);
    /*Then I create a new string of length*/
    char RealCityCode [MAX_CITYCODE_LEN+1];
    int	size = strlen(flight.arrival_city),i;
    if(size>MAX_CITYCODE_LEN+1)
    {
        size=MAX_CITYCODE_LEN+1;
        for(i=0;i<size-1;i++)
        {
            RealCityCode[i]=flight.arrival_city[i];
        }
        RealCityCode[size-1]='\0';
        /* In order to copy this new city_code to the structure, we have to
        empty the previous city_code*/
        for(i=0;i<strlen(flight.arrival_city);i++)
        {
            flight.arrival_city[i]='\0';
        }
        /*Finally we copy the new city code to the structure*/
        strcpy(flight.arrival_city,RealCityCode);
    }
    else
    {
        for(i=0;i<size;i++)
        {
            RealCityCode[i]=flight.arrival_city[i];
        }
        RealCityCode[size]='\0';
        /* In order to copy this new city_code to the structure, we have to
        empty the previous city_code*/
        for(i=0;i<strlen(flight.arrival_city);i++)
        {
            flight.arrival_city[i]='\0';
        }
        /*Finally we copy the new city code to the structure*/
        strcpy(flight.arrival_city,RealCityCode);
    }
    repeat=0;
	/*I ask only once this so that I don't put this message again in case of
	wrong entry by the user.*/
    printf("Enter arrival info.\n");
    do
	{
	    if(repeat!=0)
		{
		    printf("Invalid input\n");
		}
	    printf("Enter month, date, hour and minute separated by spaces>\n");
	    scanf("%d %d %d %d", &flight.arrival_dt.month, &flight.arrival_dt.day,
	    &flight.arrival_dt.hour, &flight.arrival_dt.minute);
	    repeat++;
	}
    while(CheckDate(flight.arrival_dt.month, flight.arrival_dt.day,
    flight.arrival_dt.hour, flight.departure_dt.minute)==1);
    repeat=0;
    return flight;
}
/*******************************************************************************
 * This function prints the flights info (by calling the orther function 
 Print_Flight) of either all the flights by taping '*' or flights with specific 
 citycode by entering it while the function asks for it.
 * inputs:
 * -flight_t List_flights[MAX_NUM_FLIGHTS]: array containing all flights info
 * outputs:
 * - none
*******************************************************************************/
void Display_Flights (flight_t List_flights[],int MAX_NUM_FLIGHTS)
{
    printf("Enter arrival city code or enter * to show all destinations>\n");
    char arrival[MAX_NBR_CHARACTER];
    scanf("%s", arrival);
    int i;
    if(arrival[0]=='*')
	{
        /*if the number of flights is still equal to 0, it means there is no   
         flight in the array of flight and therefore we need to print this 
         message.*/
	    if(numberflight==0)
	    {
	        printf("No flights\n");
	    }
	    else
	    {
            /*Since the flights info must be following each other, I print this
            before entering in the loop to print the flights info*/
	        printf("Flight Origin          Destination\n");
	        printf("------ --------------- ---------------\n");
		    for(i=0;i<numberflight;i++)
		    {
			    Print_flight(List_flights[i]);
		    }
		}
	}
	else
	{
	    int repeat=0;
	    for(i=0;i<=numberflight;i++)
		{
		    if(strcmp(arrival,List_flights[i].arrival_city)==0)
			{
			    /*Again, since the flights info must be following each other, I
			    print this only once. To do it, I use my integer m initialized 
			    to 0: the first time we enter the if statement, m=0 so we print 
			    it and if we come in the if statement again, it won't be printed 
			    a second time because m would have been incremented before*/
			    if(repeat==0)
			    {
			        printf("Flight Origin          Destination\n");
	                printf("------ --------------- ---------------\n");
			    }
			    Print_flight(List_flights[i]);
			    repeat++;
			}
		}
        /*If we never entered the if statement above, meaning there are no 
        flight with the same citycode, then m would still be equal to 0 which 
        would allow the programm to enter this next if statement to print 
        this message.*/
	    if(repeat==0)
		{
		    printf("No flights\n");
		}
	}
}

/*******************************************************************************
 * This function prints the flights info.
 * inputs:
 * -flight_t flight
 * outputs:
 * - none
*******************************************************************************/
void Print_flight(flight_t flight)
{

    printf("%s", flight.flightcode);
    /*After printing the flightcode, we need to know exactly how many spaces we
    need to put after to have the same display for each flight independently of 
    the length of the string flightcode. Then we create the integer difference 
    equals to MAX_FLIGHTCODE_LEN- the actual length of the flightcode entered by 
    the user*/
    int difference=MAX_FLIGHTCODE_LEN-strlen(flight.flightcode);
    int i;
    /*And for each difference, we print a space.*/
    for(i=0;i<=difference;i++)
    {
        printf(" ");
    }
    printf("SYD %02d-%02d %02d:%02d ",
    flight.departure_dt.month,
    flight.departure_dt.day,
    flight.departure_dt.hour,
    flight.departure_dt.minute);
    printf("%s",flight.arrival_city);
    /*Same procedure than above for flightcode.*/
    difference=MAX_CITYCODE_LEN-strlen(flight.arrival_city);
    for(i=0;i<=difference;i++)
    {
        printf(" ");
    }
    printf("%02d-%02d %02d:%02d\n",
    flight.arrival_dt.month,
    flight.arrival_dt.day, 
    flight.arrival_dt.hour, 
    flight.arrival_dt.minute);
}

/*******************************************************************************
 * This function takes the information of the flights in the array of flights 
 and write it in a file.
 * -flight_t List_flights[MAX_NUM_FLIGHTS]: array containing all flights info
 * outputs:
 * - none
*******************************************************************************/
void SaveDatabase(flight_t List_flights[],int MAX_NUM_FLIGHTS)
{
    FILE* file = NULL;
    file = fopen(DB_NAME, "w");
    if(file==NULL)
	{
        printf("Error! Can't open file.\n");
	}
    else
	{
	    int i;
		for(i=0;i<numberflight;i++)
		{
		    fprintf(file," %s %02d %02d %02d %02d %s %02d %02d %02d %02d",
		    List_flights[i].flightcode, 
		    List_flights[i].departure_dt.month,
		    List_flights[i].departure_dt.day,
		    List_flights[i].departure_dt.hour,
            List_flights[i].departure_dt.minute,
            List_flights[i].arrival_city,
            List_flights[i].arrival_dt.month, 
            List_flights[i].arrival_dt.day,
            List_flights[i].arrival_dt.hour,
            List_flights[i].arrival_dt.minute);
            if(i<numberflight-1)
            {
                fprintf(file,"\n");
            }
		}
	    fclose(file);
	}
}

/*******************************************************************************
 * This function reads the information of flights in a file already filled with
 flights information and puts those values into actual variable flight in the
 array of flights.
 After opening the file, i enter a while loop which continues until the number 
 of flights read becomes higher than the number max of flights the array can 
 contain or we reach the end of the file (EOF).
 I then use the fscanf function in the loop before incrementing numberflights.
 * -flight_t List_flights[]: array containing all flights info
 * -Length of the array
 * outputs:
 * - none
*******************************************************************************/
void LoadDatabase(flight_t List_flights[], int MAX_NUM_FLIGHTS)
{
    FILE* file = NULL;
    file = fopen(DB_NAME, "r");
    if(file==NULL)
	{
	    printf("Read error\n");
	}
    else
	{
	    while(numberflight<MAX_NUM_FLIGHTS && fgetc(file)!=EOF)
        {
            fscanf(file,"%s %02d %02d %02d %02d %s %02d %02d %02d %02d",
            List_flights[numberflight].flightcode,  
            &List_flights[numberflight].departure_dt.month,
            &List_flights[numberflight].departure_dt.day,
            &List_flights[numberflight].departure_dt.hour,
            &List_flights[numberflight].departure_dt.minute, 
            List_flights[numberflight].arrival_city,
            &List_flights[numberflight].arrival_dt.month, 
            &List_flights[numberflight].arrival_dt.day,
            &List_flights[numberflight].arrival_dt.hour,
            &List_flights[numberflight].arrival_dt.minute);
            numberflight++;
        }
	    fclose(file);
	}
}

/*******************************************************************************
 * This function checks if values entered by the user are valid.
First, it verifies if the month is an integer between 1 and 12.
If the user entered 4 for the month, he won't be able to enter 31 as the number
of days as there is not 31 days in April.
Furthermore, it checks if the value of hour is between 0 and 23 and minutes
between 0 and 59.
In order to do it, I put in parameters the month, day, hour and minute entered
by the user; I initialize the boolean integer to true (ie 0) and I put
'if condition' which, if not complete change the boolean to false (ie 1);
Like this, if only one of the conditions isn't verified, the function returns
false.
 * inputs:
 * - integer month, day, hour, minute
 * outputs:
 * - integer 0 (true) or 1 (false)
*******************************************************************************/
int CheckDate(int month, int date, int hour, int minute)
{
    int right=0;
    if(1>month || month>12)
	{
	    right=1;
	}
    if(month ==1 || month == 3 || month == 5 || month == 7 || month == 8
    || month == 10 || month == 12)
	{
	    if(1>date || date>31)
		{
		    right=1;
		}
	}
    else
	{
	    if(month==2)
		{
		    if(1>date || date>29)
			{
			    right=1;
			}
		}
	    else
		{
		    if(1>date || date>30)
			{
			    right=1;
			}
		}
	}
    if(0>hour || hour>23)
	{
	    right=1;
	}
    if(0>minute || minute>59)
	{
	    right=1;
	}
    return right;
}

/*******************************************************************************
 * This function checks if the flightcode entered by the user is composed by two
uppercase letters and between 1 and 4 integers after.
I set the value of the boolean integer to true at the beginning and if at least 
one condition isn't fulfilled, I turn the value to false (ie 1):
- I check the number of characters of the string which must be between 3 and 6
- Then with the ASCII code of uppercase characters, I check if the two first
characters of the flightcode are uppercase letters.
- Finally and similarly, I check if the last characters of the flightcode 
(3 to 6 or 3 to 4 ...) are numbers between 0 and 9.
 * inputs:
 * - string flightcode
 * outputs:
 * - integer 0 (true) or 1 (false)
*******************************************************************************/
int CheckFlightCode(char* code)
{
    int right =0, i;
    if(strlen(code)>6 || strlen(code)<3)
    {
        right=1;
    }
    for(i=0;i<2;i++)
	{
	    if(!(65<=code[i] && code[i]<=90))
		{
		    right=1;
		}
	}
    for(i=2; i<strlen(code);i++)
	{
	    if(!(48<=code[i] && code[i]<=57))
		{
		    right=1;
		}
	}
    return right;
}
