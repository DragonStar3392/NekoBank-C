#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>//For standard I/O Operation
#include <stdlib.h>//To use functions like system(); system("dos command"); - this performs the dos command given. Eg. system("del abc.txt"); - This command deletes the file abc.txt from current location of project
#include <windows.h>//Used in making gotoxy(int, int) function
#include <conio.h>//For _getch(); _getche() type of functions, no need for enter button
#include <time.h>//To get system date and time
#include <math.h>//To calculate mathematical problems, used esp. in interest calculation
#include <string.h>//For string formats

//I've divided our user defined functions into two parts: Minor Functions (which are used frequently to support execution of core porgram) and Major Functions (which are a part of core program).  Minor function definition from Line 12-25. Major function definitions from Line 27-40.

void gotoxy(int x, int y); //To move the cursor to x column and y row of output screen
void _password(char buffer[]);// To input password from user so that asterik (*) is displayed for each character entered. Similar to gets(string); function
void rectangle(int x, int y, int l, int b);// To make a rectangle starting from (x,y) with length l and breadth b
void cls(int x1, int y1, int x2, int y2); // To clear only certain portion of screen starting from (x1,y1) to (x2,y2). Notice this function has been used in date entry. When date is not in dd/mm/yyyy format, the wrong value is cleared on the screen
void increase(char i_id[]); // To increase ID by unit value. Eg. U001 ---> U002 and AC00099 ---> AC00100. Since i_id is an array, it is passed by reference. So the input value and changed value is obtained from  same variable i_id[]
void date_input(char date[]); // To convert date in format dd/mm/yyyy (The format in which user is asked to enter date) to mm/dd/yyyy (The format in which date is stored in data file)
void date_output(char date[]); //To convert date in format mm/dd/yy to dd MMM, yyyy. Eg. 01/01/11 is changed to 01 Jan, 2011
int date_check(char date[]);//Function to check if date is in format dd/mm/yyyy or not. Returns 0 if the format is not valid and 1 for valid format.
void title();//Clears the screen and displays Program title, Current User and Current date on top of Screen
void welcome();// Displays welcome screen that you see as soon as program is executed
void printFormat(double n); //convert and print into baht form, though sataang is not yet implemented.

void customer();//Main program for user portion to manage accounts
void uptodate();// This function updates every account by calculating interest and stores the updated value. This function runs every time the program is executed.
void user_register();// To create a new user to login
void deposit(); // To deposit cash in anyone's account
void withdraw(); // To withdraw cash from user's account
void user_info();// To view informations about the user

char date[13], ttime[10]; // These are global variables to store system date and time

FILE *fp, *fp1, *fp2, *tfp; // File pointers

struct user { //Structure for storing User information

	char uid[4];
	char name[50], password[50];

	char money[50];
	double money2;
	char baht[5];
	char u_date[15];

};

struct {//Structure for storing account information

	char ac_no[4];
	char name[50];
	char u_date[15];

}account;

int gbl_flag = 0;

int main() {
	char c;
	welcome(); //Welcome screen at beginning
	do {
		system("cls"); //Clears the screen
		rectangle(8, 9, 70, 13);
		gotoxy(12, 11); printf("Press L to log in, R to register, Q or ESC key to quit\n\n\n\t\t\t\t\t");
		c = _getche(); //Variable c stores the key pressed by user

		if (c == 'L' || c == 'l') {
			customer();
			break;
		}
		if (c == 'R' || c == 'r') {
			user_register();
		}
		if (c == 'Q' || c == 'q')exit(0);
		if (c == 27) exit(0); //27 is ASCII code of escape key, means program exits when user presses Esc key insted to A or S

	} while (1); //infinite loop incase any key other than Esc or L/l is pressed.

	return 0;
}

void customer() {
	struct user login;
	int i;
	//Log in Screen begins...

	char pass[30], passwrd[50] = { 0 }, id[10], nam[30],ch;
	int c = 0, cnt = 0;
	char ex, stime[9], etime[9];

	cnt = 0;//This variable cnt counts the number of attempts of Log in
	while (1) {//incase the user is an idiot
		do {
			system("cls");
			rectangle(10, 8, 70, 15);
			gotoxy(7, 5); printf("Only FIVE attempts shall be allowed to enter username and password.");
			gotoxy(7, 6); printf("Press R to return. Others to continue.");
			gotoxy(7, 20); printf("P.s. Username and password must not exceed 30 characters.");
			ch = _getch();
			if (ch == 'R' || ch == 'r')
				main();

			gotoxy(23, 10); printf("Enter Username  : "); scanf("%s", login.name);
			if (strlen(login.name) > 30) {
				gotoxy(23, 14); printf("Username characters exceeds the limit.");
				_getch();
				break;
			}
			gotoxy(23, 12); printf("Password        : ");
			char passwrd[30] = { 0 };//To nullify the string passwrd
			_password(passwrd);
			strcpy(login.password, passwrd);
			if (strlen(login.password) > 30) {
				gotoxy(23, 14); printf("Password characters exceeds the limit.");
				_getch();
				break;
			}
			cnt++;

			if (cnt == 5) {// when no of attempts exceeds 3

				title();
				gotoxy(25, 10); printf("Invalid User name or Password!\a");
				gotoxy(22, 12); printf("Press ENTER to exit the program...");
				_getch();
				exit(0);
			}

			c = 0;//Counts the no. of user for which the name and password matches
			fp = fopen("USER.txt", "r");
			while (fscanf(fp, "%s %s %s\n", id, nam, pass) != EOF) {

				if ((strcmp(login.name, nam) == 0) && (strcmp(login.password, pass) == 0)) //if the combination matches value of c is increased and user id of req user is stored in user.id
				{
					c++;
					strcpy(login.uid, id);
					strcpy(account.name, login.name);
				}
			}

			fclose(fp);

			title();
			if (c == 0) {// when no records are found c=0

				gotoxy(10, 10); printf("Cannot find the given combination of USER NAME and PASSWORD!\a");
				_getch();
			}
			else break; //terminates do... while loop if record found

		} while (1);//not exactly infinite as user is prompted only maximum three times but the terminating conditions are on line 146 and 176
		_strtime(stime);
		break;
	}
	//Log in screen ends

	//Main Menu begins//
	uptodate(); // To update interests
	do {
		title();

		gotoxy(15, 8); printf("1. Cash Deposit");
		gotoxy(15, 10); printf("2. Cash Withdraw");
		gotoxy(15, 12); printf("3. Account information");
		gotoxy(45, 8); printf("4. Log out");
		gotoxy(45, 10); printf("5. Exit");

		gotoxy(1, 17); for (i = 0; i<78; i++) printf("_");
		gotoxy(23, 19); printf("Press a choice between the range [1-5] ");

		switch (_getch() - 48) {//check if in range/number or not, 48 = '0' for ascii
		case 1:
			deposit();
			break;

		case 2:
			withdraw();
			break;

		case 3:
			user_info();
			break;

		case 4:
			title();
			gotoxy(15, 10); printf("Are you sure you want to Log out? <Y/N> : ");
			ex = _getche();
			if (ex == 'Y' || ex == 'y') {
				//if user wants to log out all user log information are stored in LOG.txt and the function customer() called again i.e. log in screen begins again
				_strtime(etime);
				fp = fopen("LOG.txt", "a");
				fprintf(fp, "%s %s %s %s\n", login.uid, date, stime, etime);
				fclose(fp);
				customer();
			}
			break;

		case 5:
			title();
			gotoxy(15, 10); printf("Are you sure you want to exit? <Y/N> : ");
			ex = _getche();
			if (ex == 'Y' || ex == 'y') {
				//if user wants to exit, all user log information are stored in LOG.txt and the program is terminated
				_strtime(etime);
				fp = fopen("LOG.txt", "a");
				fprintf(fp, "%s %s %s %s\n", login.uid, date, stime, etime);
				fclose(fp);
				exit(0);
			}
			break;

		default://when entered characted is not between 1-5
			title();
			gotoxy(10, 10); printf("Your input is out of range! Enter a choice between 1 to 5!");
			gotoxy(15, 12); printf("Press any key to return to main menu...");
			_getch();
		}
	} while (1);//infinite loop to return to main menu after execution of any function
				//return 0;
}

void uptodate() {
	struct user save;
	int i, no_of_yr, no_of_month, no_of_days, n1, n2;

	fp = fopen("DUMMY.txt", "r");

	if (fp != NULL)//performs every thing only when file exists i.e. only when pointer is not null
	{
		fp1 = fopen("TEMP.txt", "w");
		i = 0;
		while (fscanf(fp, "%s %s %s %s\n", save.uid, save.name, save.money, save.u_date) != EOF)//A%s %s %f %s %c
		{
			//extracting no of days, months and yr of system date
			no_of_yr = (date[6] - 48) * 10 + (date[7] - 48);
			no_of_month = (date[0] - 48) * 10 + (date[1] - 48);
			no_of_days = (date[3] - 48) * 10 + (date[4] - 48);

			n1 = no_of_yr * 365 + no_of_month * 30 + no_of_days;//n1 is no. of days elasped

																//extracting no of days, months and yr of date stored in ACCOUNT.txt
			no_of_yr = (save.u_date[6] - 48) * 10 + (save.u_date[7] - 48);
			no_of_month = (save.u_date[0] - 48) * 10 + (save.u_date[1] - 48);
			no_of_days = (save.u_date[3] - 48) * 10 + (save.u_date[4] - 48);
			n2 = no_of_yr * 365 + no_of_month * 30 + no_of_days;//n2 is no. of days elasped on last updated date

			strcpy(date, save.u_date);
			fprintf(fp1, "%s %s %s %s\n", save.uid, save.name, save.money, save.u_date);

		}

		fclose(fp);
		fclose(fp1);
		system("del DUMMY.txt");
		system("ren TEMP.txt DUMMY.txt");
	}


}

void user_register() {
	struct user regist;
	char ch;
	char passwrd[50] = { 0 }, password[30], nam[50], namR[30], origi[30];
	int c = 0, cnt = 0;
	regist.money2 = 0.00;

	uptodate();//for today's date 
	while (1) {//No idiots allowed
		while (1) {
			system("cls");
			rectangle(10, 8, 70, 15);
			gotoxy(25, 11); printf("Want to register a new account?");
			gotoxy(38, 13); printf("Y/N");

			ch = _getche();
			if (ch == 'Y' || ch == 'y') {

				system("cls");
				rectangle(10, 8, 70, 15);
				gotoxy(7, 5); printf("Making new user: ");
				gotoxy(23, 10); printf("Enter User name : "); scanf("%s", nam);
				if (strlen(nam) > 30) {
					gotoxy(7, 10); printf("Username characters exceeds the limit.");
					_getch();
					break;
				}
				strcpy(origi, nam);

				gotoxy(23, 12); printf("Password        : ");
				char passwrd[50] = { 0 };//To nullify the string password
				_password(passwrd);
				strcpy(password, passwrd);
				if (strlen(password) > 30) {
					gotoxy(7, 10); printf("Username characters exceeds the limit.");
					_getch();
					break;
				}

				fp = fopen("USER.txt", "r");
				if (fp == NULL) strcpy(regist.uid, "U01");//if "ACCOUNT.txt" does not exist i.e. there are no records at all then A/C no. is taken U01 for 1st rec
				else //otherwise a/c no of last record is accessed and increased by unit value which becomes the new a/c no.
				{
					while (fscanf(fp, "%s%*c%s%*c%s\n", regist.uid, regist.name, regist.password) != EOF) {
						strcpy(namR, _strupr(regist.name));
						if (strcmp(namR, nam) == 0) {//checks for same username
							c = 0;
						}
						else
							c = 1;
					}
					increase(regist.uid);//function increase number
				}
				fclose(fp);

				if (c == 0) {
					system("cls");
					gotoxy(20, 10); printf("Invalid Username, press any key to continue...\a");
					_getche();
					return;
				}

				fp = fopen("USER.txt", "a");
				fprintf(fp, "%s%c%s%c%s\n", regist.uid, ' ', origi, ' ', password);
				fclose(fp);

				fp = fopen("ACCOUNT.txt", "a");
				fprintf(fp, "%s %s %.2f %s\n", regist.uid, nam, regist.money2, date);
				fclose(fp);

				system("cls");
				gotoxy(15, 8); printf("Guuddojobbu!");
				gotoxy(15, 10); printf("Account creation successful! Press any key to continue...\a");
				_getche();
				return;

			}
			else if (ch == 'N' || ch == 'n')
				return;
			else {
				gotoxy(15, 10); printf("What are you even trying? nyaa?");
				_getch();
			}
		}
		break;
	}
}

void deposit() {
	struct user depo;
	struct user save;
	save.money2 = 0.00; //assign so don't have error, initally save.money2 has no value and it's double

	title();
	char acn[10], ch, nam[35];
	int c = 0, ret = 0;
	double amt = 0.00;
	char sub[50]; //substitute for amount in string

	gotoxy(5, 10); printf("Deposit to username(e.g. Neko)            : "); scanf("%s", acn);//Deposit to your or other users

	fp = fopen("ACCOUNT.txt", "r");
	while (fscanf(fp, "%s %s %s %s\n", depo.uid, depo.name, depo.money, depo.u_date) != EOF) {
		if (strcmp(acn, depo.name) == 0) {//found same desired user and input user
			c++;
			strcpy(nam, depo.name);
			break;
		} //variable c counts if the given a/c no exist in data file or not. also if available, the full name is extracted
	}

	fclose(fp);

	if (c == 0) {//c=0 means no given user no. in data file
		title();
		gotoxy(20, 12); printf("Given user does not exists!");
		_getch();
		return;
	}

	gotoxy(49, 10); printf("[ %s ]", nam);
	gotoxy(5, 12); printf("Amount to be deposited (with or w/o decimal) : ");
	while (1) {
		scanf("%s", &sub);
		amt = atof(sub);

		if (amt > 2147483647.999999) {
			gotoxy(5, 14); printf("The amount is too large! You stole the money didn't ya? ");
			_getch();
			return;
		}

		while (getchar() != '\n');
		if (amt <= 0) { //check input is < 1 or not number(s)
			gotoxy(5, 14); printf("Wrong input data type!");
			_getch();
			title();
			gotoxy(5, 12); printf("Amount to be deposited (with decimal or no decimal) : ");
		}
		else
			break;
	}

	title();
	gotoxy(30, 10); printf("Confirm Transaction");
	gotoxy(35, 12); printFormat(amt);
	gotoxy(20, 14); printf(" to be deposited into user : [ %s ]", nam);

	gotoxy(8, 17); printf("Are you sure you want to perform this transaction? <Y/N>");


	while (1) {
		ch = _getche();
		if (ch == 'Y' || ch == 'y') {
			fp = fopen("ACCOUNT.txt", "r");
			fp1 = fopen("TEMP.txt", "w");

			while (fscanf(fp, "%s %s %s %s\n", save.uid, save.name, save.money, save.u_date) != EOF) {
				//gotoxy(35, 11); printf("Test test");
				save.money2 = atof(save.money);
				if (strcmp(save.name, acn) == 0) {
					save.money2 += amt;//balance is increased
					fprintf(fp1, "%s %s %.2lf %s\n", save.uid, save.name, save.money2, date);
					break;
				}
				else {
					fprintf(fp1, "%s %s %.2lf %s\n", save.uid, save.name, save.money2, date);
				}
			}

			fclose(fp1);
			fclose(fp);
			ret = remove("ACCOUNT.txt");
			if (ret == 0) {
				gotoxy(8, 20); printf("Success at deleting");
			}
			else {
				gotoxy(8, 20); printf("Failed at deleting");
			}
			rename("TEMP.txt", "ACCOUNT.txt");


			fp = fopen("TRANSACTION.txt", "a");//transaction is added
			_strtime(ttime);
			fprintf(fp, "%s %s %s %.2lf %s %s\n", acn, "CashDepositedTo", save.name, amt, date, ttime);
			fclose(fp);
			title();
			gotoxy(20, 12); printf("Transaction completed successfully nyaa!");
			gotoxy(20, 14); printf("Thank kyu for your money to feed us cats. :3");
			_getch();
			return;
		}
		else if (ch == 'N' || ch == 'n') {
			while (getchar() != '\n');
			return;
		}
		else {
			system("cls");
			gotoxy(25, 10); printf("Invalid input!");
			_getche();
			
		}
	}
}

void withdraw() {
	struct user save;
	save.money2 = 0.00; //assign so don't have error, initally save.money2 has no value and it's double

	title();
	char ch, nam[30], sub[50];
	double amt = 0.00; int ret = 0;

	strcpy(nam, account.name);


	gotoxy(49, 10); printf("[ %s ]", nam);
	gotoxy(5, 12); printf("Amount to be withdrawn (with or w/o decimal) : ");
	while (1) {
		scanf("%s", &sub);
		amt = atof(sub);
		if (amt > 2147483647.999999) {
			gotoxy(5, 14); printf("The amount is too large! You stole the money didn't ya? ");
			_getch();
			return;
		}
		while (getchar() != '\n');
		if (amt <= 0) { //check input is < 1 or not number(s)
			gotoxy(5, 14); printf("Wrong input data type or negative input!");
			title();
			gotoxy(49, 10); printf("[ %s ]", nam);
			gotoxy(5, 12); printf("Amount to be withdrawn (with decimal or no decimal) : ");
		}
		else
			break;
	}

	title();
	gotoxy(30, 10); printf("Confirm Transaction");
	gotoxy(35, 12); printFormat(amt);
	gotoxy(20, 14); printf(" to be withdrawn from user : [ %s ]", nam);

	gotoxy(8, 17); printf("Are you sure you want to perform this tranasction? <Y/N>");

	while (1) {
		ch = _getche();
		if (ch == 'Y' || ch == 'y') {
			fp = fopen("ACCOUNT.txt", "r");
			fp1 = fopen("TEMP.txt", "w");

			while (fscanf(fp, "%s %s %s %s\n", save.uid, save.name, save.money, save.u_date) != EOF) {
				save.money2 = atof(save.money);
				if (strcmp(save.name, nam) == 0) {
					if (save.money2 >= amt) {
						save.money2 -= amt;//balance is decreased
						fprintf(fp1, "%s %s %.2lf %s\n", save.uid, save.name, save.money2, date);
						break;
					}
					else {
						gotoxy(8, 20); printf("Unable to withdraw due to the withdraw exceeds what is in the account.");
						_getche();
						return;
					}
				}
				else {
					fprintf(fp1, "%s %s %.2lf %s\n", save.uid, save.name, save.money2, date);
				}
			}

			fclose(fp1);
			fclose(fp);

			ret = remove("ACCOUNT.txt");
			if (ret == 0) {
				gotoxy(8, 20); printf("Success at deleting");
			}
			else {
				gotoxy(8, 20); printf("Failed at deleting");
			}
			rename("TEMP.txt", "ACCOUNT.txt");

			fp = fopen("TRANSACTION.txt", "a");//transaction is added
			_strtime(ttime);
			fprintf(fp, "%s %s %s %.2lf %s %s\n", nam, "CashWithdrawnFrom", save.name, amt, date, ttime);
			fclose(fp);
			title();
			gotoxy(20, 12); printf("Transaction completed successfully nyaa!");
			gotoxy(20, 14); printf("Will you buy us cats cat food? :3");
			_getch();
			return;
		}
		else if (ch == 'N' || ch == 'n') {
			while (getchar() != '\n');
			return;
		}
		else {
			title();
			gotoxy(25, 10); printf("Invalid input!");
			_getch();
			while (getchar() != '\n');
		}
	}
}

void user_info() {
	struct user look;

	char acn[30];//temp1 for look.name
	char c;

	int cnt, i = 0; double fmt = 0.00;

	title();
	gotoxy(27, 8); printf("1. Continue");
	gotoxy(27, 10); printf("2. Return to main menu");
	gotoxy(25, 16); printf("Press a choice between 1 or 2: ");
	c = _getche();

	switch (c - 48) {
	case 1:

		title();

		strcpy(acn, account.name);
		fp = fopen("ACCOUNT.txt", "r");
		cnt = 0;

		while (fscanf(fp, "%s %s %s %s\n", look.uid, look.name, look.money, look.u_date) != EOF) {
			if (strcmp(acn, look.name) == 0) {
				fmt = atof(look.money);
				title();
				gotoxy(3, 9); printf("User ID        : %s", look.uid);
				gotoxy(3, 11); printf("Username      : %s", look.name);
				gotoxy(3, 13); printf("Money in account   : "); printFormat(fmt);
				gotoxy(3, 15); printf("Latest active date (mm/dd/yy) : %s", look.u_date);
				cnt++;
				break;
			}
		}

		if (cnt == 0) {
			fclose(fp);
			title();
			gotoxy(20, 12); printf("Given user does not exits!");
			_getch();
			return;
		}
		fclose(fp);
		gotoxy(3, 20); printf("Press any key to return...(Not your house or car key!)");
		_getche();

		return;

	case 2:
		return;

	default:
		title();
		gotoxy(10, 10); printf("Your input is out of range! Enter a choice between 1 or 2!");
		gotoxy(15, 12); printf("Press any key to return to try again...");
		gotoxy(15, 15); printf("Why have you dissapointed us? nyaaa");
		_getch();
		user_info();

	}
}
//------------------------------------------------------------------------------------------------

void welcome()
{
	int i;
	system("cls");
	rectangle(0, 0, 80, 23);
	gotoxy(23, 4); printf("NekoBank V1.0.0 (ATM Simulator)");
	gotoxy(23, 5); for (i = 0; i<31; i++) printf("%c", 196);
	gotoxy(25, 8); printf("Designed and Programmed by:");
	gotoxy(25, 9); for (i = 0; i<27; i++) printf("%c", 196);
	gotoxy(33, 11); printf("Shouh Yann Mo");
	gotoxy(33, 12); for (i = 0; i<13; i++) printf("%c", 196);
	gotoxy(26, 13); printf("ID: 59090029, KMITL Year 1");
	gotoxy(26, 14); for (i = 0; i<26; i++) printf("%c", 196);
	gotoxy(23, 16); printf("Puck GUI, looks better like this.");
	gotoxy(31, 17); printf("Nyaaa, welcome! :3");

	gotoxy(30, 19); printf("%c By creator in 2016",153);
	gotoxy(24, 20); printf("Press Any key to continue...");


	_getch();

}

void date_input(char date[]) // dd/mm/yyyy ----> mm/dd/yy
{
	char temp[15];

	temp[3] = date[0];
	temp[4] = date[1];
	temp[0] = date[3];
	temp[1] = date[4];
	temp[6] = date[8];
	temp[7] = date[9];
	temp[8] = '\0';
	temp[2] = temp[5] = '/';

	strcpy(date, temp);
}

void date_output(char date[]) //mm/dd/yy ---> dd MMM, yyyy
{
	struct
	{
		int dd;
		char mm[4];
		int yyyy;
	}_date;

	char temp[15];
	int mm, c, i;
	_date.dd = (date[3] - 48) * 10 + (date[4] - 48);

	mm = (date[0] - 48) * 10 + (date[1] - 48);
	_date.yyyy = 2000 + (date[6] - 48) * 10 + (date[7] - 48);
	switch (mm)
	{
	case 1: strcpy(_date.mm, "Jan"); break;
	case 2: strcpy(_date.mm, "Feb"); break;
	case 3: strcpy(_date.mm, "Mar"); break;
	case 4: strcpy(_date.mm, "Apr"); break;
	case 5: strcpy(_date.mm, "May"); break;
	case 6: strcpy(_date.mm, "Jun"); break;
	case 7: strcpy(_date.mm, "Jul"); break;
	case 8: strcpy(_date.mm, "Aug"); break;
	case 9: strcpy(_date.mm, "Sep"); break;
	case 10: strcpy(_date.mm, "Oct"); break;
	case 11: strcpy(_date.mm, "Nov"); break;
	case 12: strcpy(_date.mm, "Dec"); break;
	}

	temp[0] = (int)(_date.dd / 10) + 48;
	temp[1] = (int)(_date.dd % 10) + 48;
	temp[2] = 32;
	temp[3] = '\0';
	strcat(temp, _date.mm);
	temp[6] = ',';
	c = 0;
	temp[7] = 32;
	for (i = 3; i >= 0; i--)
	{
		temp[8 + c] = (int)(_date.yyyy / pow(10, i)) + 48;
		c++;
		_date.yyyy %= (int)pow(10, i);
	}
	temp[12] = '\0';
	strcpy(date, temp);
}

int date_check(char _date[]) //_date[] is in format dd/mm/yyyy
{
	int err = 0, mm, dd, yyyy;
	if (strlen(_date) != 10) err++;
	if (_date[2] != '/' || _date[5] != '/') err++;
	mm = (_date[3] - 48) * 10 + (_date[4] - 48);
	if (mm>12) err++;
	dd = (_date[0] - 48) * 10 + (_date[1] - 48);

	yyyy = (_date[6] - 48) * 1000 + (_date[7] - 48) * 100 + (_date[8] - 48) * 10 + (_date[9] - 48);

	switch (mm) {
	case 1: if (dd>31) err++; break;
	case 2:
		if (dd>28) err++;
		if (yyyy % 4 == 0 && dd == 29) err--;
		break;
	case 3: if (dd>31) err++; break;
	case 4: if (dd>30) err++; break;
	case 5: if (dd>31) err++; break;
	case 6: if (dd>30) err++; break;
	case 7: if (dd>31) err++; break;
	case 8: if (dd>31) err++; break;
	case 9: if (dd>30) err++; break;
	case 10: if (dd>31) err++; break;
	case 11: if (dd>30) err++; break;
	case 12: if (dd>31) err++; break;

	}

	if (err == 0)
		return 1;
	else
		return 0;

}

void printfcomma(double n) {
	int x;
	x = (int)n;
	char temp[100];
	sprintf(temp, "%d", x);
	int i;
	int left = strlen(temp);
	for (i = 0; i<strlen(temp); i++) {
		printf("%c", temp[i]);
		if (left % 3 == 1 && i != strlen(temp) - 1)
			printf(",");
		left--;
	}
}
void printfdecimal(double n) {
	char temp[10];
	sprintf(temp, "\n%.2f", fmod(n, 1));
	int i;
	for (i = 2; i<strlen(temp); i++) {
		printf("%c", temp[i]);
	}
	printf(" Baht");
}
void printFormat(double n) {
	printfcomma(n);
	printfdecimal(n);
}

void cls(int x1, int y1, int x2, int y2)
{
	int i, j;
	for (i = y1; i <= y2; i++)
		for (j = x1; j <= x2; j++)
		{
			gotoxy(j, i); printf("%c", 32);
		}

}

void increase(char i_id[])
{
	int l;
	l = strlen(i_id) - 1;
	do
	{
		if (i_id[l] != '9') i_id[l] += 1;
		else i_id[l] = '0';
		l--;

	} while (i_id[l + 1] == '0');


}

int num_of_rec(char file[], int n) {
	FILE *fptr;
	char dat[200];
	long int c = 0;
	fptr = fopen(file, "r");
	while (fscanf(fptr, "%s", dat) != EOF) c++;
	fclose(fptr);
	return c / n;

}

void _password(char buffer[]) {

	char c;
	int pos = 0;
	do
	{
		c = _getch();
		if (isprint(c))
		{
			buffer[pos++] = c;
			printf("%c", '*');
		}
		else if (c == 8 && pos)
		{
			buffer[pos--] = '\0';
			printf("%s", "\b \b");
		}
	} while (c != 13);



}

void rectangle(int x, int y, int l, int b) {
	int i, m;
	gotoxy(x, y); printf("%c", 201);
	for (i = x + 1; i<l - 1; i++)
	{
		gotoxy(i, y);
		printf("%c", 205);
	}
	gotoxy(i, y); printf("%c", 187);

	for (m = y + 1; m<b; m++)
	{
		gotoxy(x, m);
		for (i = x; i<l; i++)
		{
			if (i == x || i == l - 1)
			{
				gotoxy(i, m); printf("%c", 186);
			}

		}

	}

	gotoxy(x, m); printf("%c", 200);
	for (i = x + 1; i<l - 1; i++)
	{
		gotoxy(i, m);
		printf("%c", 205);
	}
	gotoxy(i, m); printf("%c", 188);
}

void title() {
	int i;

	system("cls");

	rectangle(0, 0, 80, 23);
	gotoxy(25, 1);
	printf("NekoBank V1.0.0 (ATM Simulator)");
	gotoxy(5, 3);
	printf("Current User : %s", account.name);
	printf("\t\t\t\tDate : ");
	_strdate(date);
	date_output(date);
	printf("%s", date);
	_strdate(date);
	gotoxy(1, 5);
	for (i = 0; i<78; i++) printf("%c", 196);

}

COORD coord = { 0, 0 };

void gotoxy(int x, int y) {
	coord.X = x;
	coord.Y = y; // X and Y coordinates
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}