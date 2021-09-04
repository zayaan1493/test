/*
Name : server.c
Creator : Md Laadla (mailzayaan1493.ml@kgpian.iitkgp.ac.in)
Roll No : 20CS60R20
Date : 12th January 2021
Description : This is the server code in which the client requests some command and the server execute the command and sent the
answer or some appropriate message in case of errors.
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<math.h>
#define max 1000 // max length of the sent and recieved strings between server and client and also the max size of the stacks used



  /*Operand Stack description*/

struct operand_stack{
	int top;
	double num[max];
}typedef opand_s;

   /*Operator Stack description*/
struct operator_stack{
	int top;
	char operators[max];
}typedef optor_s;



void push_opand(opand_s *st, double data){ st->num[++(st->top)]=data; }// Push in the operand stack

double pop_opand(opand_s *st){ return (st->num[(st->top)--]); }// Pop from the operand stack

void push_optor(optor_s *st, char ch){ st->operators[++(st->top)]=ch; }// Push in the operator stack

char pop_optor(optor_s *st){ return (st->operators[(st->top)--]); }// Pop from the operator stack




int is_digit_operator(char ch)
{
	/*
	Input : a character
	Return/ Output : 1 --> in case of digits or .
					 2 --> in case of operators.
					 -1 --> in case of space, ')' and new line
					 0 --> otherwise
	*/
	if ((ch>='0' && ch<='9') || ch=='.') return 1;
	else if (ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='%' || ch=='^') return 2;
	else if (ch==' ' || ch== ')' || ch=='\n' || ch=='(') return -1;
	else return 0;
}



int precedence(char ch)
{
	/*
	Input : a character
	Return : The precedence of the given character
	*/
	if (ch=='+' || ch=='-') return 1;
	else if (ch=='*' || ch=='/' || ch=='%') return 2;
	else if (ch=='^') return 3;
	else return 0;
}


double result(double op1, double op2, char ch)
{
	/*
	Inputs : op1 --> the first operand
			 op2 --> the second operand
			 ch --> the operator to be performed on these operators
	Return : The appropriate result
	*/
	if (ch=='+') return (op1+op2);
	else if (ch=='-') return (op1-op2);
	else if (ch=='*') return (op1*op2);
	else if (ch=='/') return (op1/op2);
	else if (ch=='%') return (fmod(op1,op2));
	else if (ch=='^') return (pow(op1,op2));
}


/*.......................Basic Stack Operation for both the stack (operator and operand)......................................*/


int check_empty_opands(opand_s *st)
{
	if (st->top==-1)
		return 1;
	return 0;
}

int check_empty_opts(optor_s *st)
{
	if (st->top==-1)
		return 1;
	return 0;
}

int check_full_opands(opand_s *st)
{
	if (st->top==max-1)
		return 1;
	return 0;
}

int check_full_opts(optor_s *st)
{
	if (st->top==max-1)
		return 1;
	return 0;
}

/*...............................................................................................................................*/



int paren_check(char str[], int *val)
{
	/*
	Input : str : a string
			val : pointer to integer which will contain how many enclosing parenthesis are present in str

	Return : 0 --> if the expression in str has valid order and number of parenthesis
			 1 --> In case the expression represented by str does not have valid order or number of parenthesis

	*/
	int start=0,end=0,y=0,flag=0,start_space=0,end_space=0;
	
	//number of parenthesis at the start
	int i=0;
	while (i<strlen(str))
	{
		if (str[i]=='(')
		{
			start++;i++;
		}
		else if (str[i]==' ' || str[i]=='\t') { i++; start_space++;}
		else if (str[i]==')')
			return 1;
		else
			break;
	}
	i=strlen(str)-1;
	while (i>=0)
	{
		if (str[i]==')')
		{
			end++;i--;
		}
		else if (str[i]==' ' || str[i]=='\t') { i--; end_space++; }
		else if (str[i]=='(')
			return 1;
		else
			break;
	}

	// printf("start %d end %d\n",start,end);
	// printf("start_space %d end_space %d\n",start_space,end_space );

	if (start==0 || end==0)
	{
		*val=0;flag=1;
	}
	
	for (int i = start+start_space; i < strlen(str)-(end+end_space); ++i)
	{
		if (str[i]==')')
		{
			if (y==0)
			{
				start--;
			}
			else y--;
		}
		else if (str[i]=='(')
		{
			y++;
		}

		if (start==0)
		{
			flag=1;
		}

		if (start+y < 0)
			return 1;
	}
	if (flag)
		*val=0;
	else
	{
		if (y>0)
			*val=end-y;
		else 
			*val=start+y;

	}
	return start+y-end;
	
}





int mystrcmp(char *a, char *b, int a1, int b1)
{
	/*
	Input : a --> a string
			b --> another string
			a1 --> number of enclosing parenthesis in string a
			b1 --> number of enclosing parenthesis in string b

	Return : 1 --> if both the strings a and b matches
			 0 --> if not

	Description : Ignoring the expression enclosing parenthesis and the spaces and tabs, string matching is performed to 
				look for any matched expression in the input file in case of WRITEX command from the client
	*/

	int flag=0;


	/*.......Calculation of the spaces at the start and end of the strings ........*/

	int start_space1=0,start_space2=0,end_space1=0,end_space2=0;
	int p=0;
	while (p<strlen(a))
	{
		if (a[p]==' ' || a[p]=='\t')
			p++;
		else 
			break;
	}
	start_space1=p;
	p=0;
	while (p<strlen(b))
	{
		if (b[p]==' ' || b[p]=='\t')
			p++;
		else 
			break;
	}
	start_space2=p;

	p=strlen(a)-1;

	while (p>=0)
	{
		if (a[p]==' ' || a[p]=='\t')
		{
			p--;end_space1++;
		}
		else 
			break;
	}
	
	p=strlen(b)-1;

	while (p>=0)
	{
		if (b[p]==' ' || b[p]=='\t')
		{
			p--;end_space2++;
		}
		else
			break;
	}

    /*.....................................................................................*/


	//printf("%d start space %d     %d end %d\n",start_space1,start_space2,end_space1,end_space2 );

	int i=a1+start_space1,j=b1+start_space2;
	// a[strlen(a)-a1]='\0';
	// b[strlen(b)-b1]='\0';
	// a=a+a1;
	// b=b+b1;
	//printf("%d i %d j\n",i,j );

	/*..... String matching operations......*/

	while (!flag && i<strlen(a)-(a1+end_space1) && j<strlen(b)-(b1+end_space2))
	{
		if (a[i]==b[j])
		{
			i++;j++;
		}
		else if (a[i]==' ' || a[i]=='\t')
		{
			i++;
		}
		else if (b[j]==' ' || b[j]=='\t')
			j++;
		else
		{
			//printf("a[i] %d b[j] %d\n",i,j );
			flag=1;// falg==1 if some correspending characters do not match
		}

	}
	//printf("%dfff\n",flag );
	while (i<strlen(a)-(a1+end_space1) && (a[i]=='\t' || a[i]==' ')) i++;
	while (j<strlen(b)-(b1+end_space2) && (b[j]=='\t' || b[j]==' ')) j++;

	//printf("%d i %d j\n",i,j );
	//printf("final i %lu final j %lu\n", strlen(a)-(a1+end_space1),strlen(b)-(b1+end_space2));

	if (flag || i!=strlen(a)-(a1+end_space1) || j!=strlen(b)-(b1+end_space2))
		return 0;
	else 
		return 1;
}






int valid(char str[], int *val)
{

	/*
	Input : str --> a string
			val --> number of enclosing parenthesis in str for the paren_check function

	Return : 0 --> if the expression is not valid
			 1 --> if the expression is valid

	Description : The expression is checked such as no two operator are adjacent, no two operand separated by a space, after 
				an operand there must be an operator or ')' or end of the string etc.......

	*/


	if (str[strlen(str)-1]=='\n') str[strlen(str)-1]='\0';// Incase the last character in the string is a new line (in case of in-file expressions)


	//printf("%s\n",str );
	//int val;
	//printf("paren_check %d \n",paren_check(str,val) );
	if (paren_check(str,val)!=0)
	{
		return 0;
	}

	int flag=0;// what has been seen in last parsing . flag=1 if an operator was seen, 2 if an operand, 
				//0 nothing has been seen i.e., to expect both an operand or operator next, -1 if last seen was an ')'

	for (int i = 0; i < strlen(str); ++i)
	{
		//flag values ------>  1-> operator 2->operand 0->antything
		
		if (str[i]==' ' || str[i]=='\t') continue;
		if (str[i]=='(')
		{
			flag=0;continue;
		}
		if (is_digit_operator(str[i])==0) return 0;
		if (flag==0)
		{
			if (is_digit_operator(str[i])==1)
			{
				
				int x=5;
				while (i<strlen(str) && (x=is_digit_operator(str[i]))==1)
				{
					i++;
				}
				if (x==0) return 0;
				i--;
				flag=1;// next to exect is an operator
				//printf("here %d\n",i );
			}
			else if (str[i]=='-' || str[i]=='+')
				flag=2;//next to expect is an operand
			else
				return 0;
		}
		else if (flag==-1)
		{
			if (is_digit_operator(str[i])==1)
			{
				
				int x=5;
				while (i<strlen(str) && (x=is_digit_operator(str[i]))==1)
				{
					i++;
				}
				if (x==0) return 0;
				i--;
				flag=1;
				//printf("here %d\n",i );
			}
			else if (is_digit_operator(str[i])==2)
				flag=2;

		}

		else if (flag==1)
		{
			if(is_digit_operator(str[i])==1)
				return 0;
			else if(is_digit_operator(str[i])==2)
				flag=2;
			else if (str[i]==')')
				flag=-1;
		}

		else if (flag==2)
		{
			if (is_digit_operator(str[i])==2 || str[i]==')')
				return 0;
			else
			{
				int x=5;
				while (i<strlen(str) && (x=is_digit_operator(str[i]))==1)
				{
					i++;
				}
				if (x==0) return 0;
				i--;
				flag=1;
				//printf("heare2 %d\n",i );

			}
		}

	}
	/* Ending must be done with an operand or ')*/
	if (flag==2 || flag==0)
		return 0;
	// else if (flag==0)
	// {
	// 		return 0;
	// }
	else 
		return 1;

}





int request(FILE *fp, char *exp)
{
	/*
	Input : fp --> a file pointer (pointing to "input.txt" file)
			exp --> pointer to an expression

	Return : 2 --> if the given expression represented by exp is invalid
			 1--> if the given expression already present in file pointed by fp
			 0 --> if exp is not present in file pointed by fp and hence appended in the file


	Description : The function checks if the exp is present in fp or not and also whether exp is valid or not
	*/


	size_t len=0;
	int val1=0,val2=0;
	if (valid(exp,&val1)==0)// check for validation of the expression exp
		return 2;

	int flag=0;
	fseek(fp,0,SEEK_SET);
	char *line=NULL;
	while (getline(&line, &len, fp)!=-1)// iterate each expression in the file
	{
		if (line[strlen(line)-1]=='\n') line[strlen(line)-1]='\0';
		

		if(valid(line,&val2)==0) continue; // if the expression in the file is valid then no need to perfrom matching
		else
		{
			if(mystrcmp(line,exp,val2,val1))
			{
				//printf("%s\n",line );
				flag=1;
				break;
			}

		}
		

		
	}
	//printf("flag %d\n",flag );

	if (flag)
		return 1;
	else
	{
		//append the expression into the file
		printf("%s\n",exp );
		fprintf(fp, "\n%s",exp);
		return 0;

	}

}








int evaluation(char str[], char ans[])
{
	/*
	Input : str --> the expression which needs to get computed
			ans --> after computing the exp(str) this string contains the answer

	Return : 0 --> if the expression referred by str is invalid infix expression
			 1 --> if the expression is computed and the result is assigned to the ans varible



	Description : The evaluation function computes the expression referenced by str and if the string is valid then puts the 
					answer in the ans string. It might happen that the expression is invalid infix expression, so it checks for 
					validity as well. Two stack algorithm is performed to fing the result of the given infix expression
	*/

	opand_s st_opand;
	optor_s st_optor;
	st_opand.top=st_optor.top=-1;


	int val=0;
	if (valid(str,&val)==0)// if expression is invalid return 0
		return 0;

	int flag=1,k=0;

	if (str[0]=='\n') return 0;// if only new line is present in the expression return 0(error)

	/* ...................handle expressions starting with '-' and '+' sign..............................*/
	
	if (str[0]=='-' || str[0]=='+')
	{
		
		char temp[20];
		int l=1,p=0;
		int x=5;
		while (l<strlen(str) && (x=is_digit_operator(str[l]))==1)
		{
			temp[p++]=str[l];
			l++;
		}
		temp[p]='\0';
		if (x==0) return 0;
		k=l;

		if (check_full_opands(&st_opand)) return 0;
		if (str[0]=='+')
			push_opand(&st_opand,1);
		else
			push_opand(&st_opand, -1);

		
		if (strtod(temp,NULL))
		{
			if (check_full_opands(&st_opand)) return 0;
			push_opand(&st_opand, strtod(temp,NULL));
		}

		if (check_full_opts(&st_optor)) return 0;
		push_optor(&st_optor, '*');
		flag=0;
	}

	/*............Iteration through the expression character by character to evaluate.............................................*/

	for (int i = k; i < strlen(str); ++i)
	{
		if (str[i]==' ' || str[i]=='\n'  || str[i]=='\t') continue;// ignore space, tabs in between expression and new line at the end

		else if (str[i]=='(')
		{
			if (check_full_opts(&st_optor)) return 0;
			push_optor(&st_optor,'(');
			flag=1;
			continue;
		}


		else if (is_digit_operator(str[i])==1)
		{
			/* code to handle the digits*/
			char temp[20];
			int l=0;
			int x=5;
			while (i<strlen(str) && (x=is_digit_operator(str[i]))==1)
			{
				temp[l++]=str[i++];
			}
			temp[l]='\0';
			if (x==0) return 0;
			i--;

			if (check_full_opands(&st_opand)) return 0;
			push_opand(&st_opand, strtod(temp,NULL));
		}


		else if (str[i]==')')
		{
			/* when ')' is encountered */
			while (st_optor.top!=-1 && st_optor.operators[st_optor.top]!='(')
			{

				if (check_empty_opands(&st_opand)) return 0;
				double item2= pop_opand(&st_opand);


				if (check_empty_opands(&st_opand)) return 0;
				double item1= pop_opand(&st_opand);

				if (check_empty_opts(&st_optor)) return 0;
				char op= pop_optor(&st_optor);

				if (check_full_opands(&st_opand)) return 0;
				push_opand(&st_opand, result(item1,item2,op));
			}

			if (check_empty_opts(&st_optor)) return 0;
			pop_optor(&st_optor);
		}

		else if (is_digit_operator(str[i])==2)
		{
			if (str[i]=='-' && flag==1)
			{
				// code to handle '-' after ')'
				char temp[20];
				int l=0;
				int x=5;
				while (i<strlen(str) && (x=is_digit_operator(str[i]))==1)
				{
					temp[l++]=str[i++];
				}
				temp[l]='\0';
				if (x==0) return 0;
				i--;

				if (check_full_opands(&st_opand)) return 0;
				push_opand(&st_opand, -1*strtod(temp,NULL));
			}

			else if (str[i]=='+' && flag==1)
			{
				// code to handle '+' after ')'
				flag=0;continue;
			}

			else
			{
				if (precedence(st_optor.operators[st_optor.top])==3 && precedence(str[i])==3)
				{
					// incase of power operator we need to push as they are right associative
					if (check_full_opts(&st_optor)) return 0;
					push_optor(&st_optor, str[i]);
				}

				else
				{
					while (st_optor.top!=-1 && precedence(st_optor.operators[st_optor.top])>=precedence(str[i]))
					{
						if (check_empty_opands(&st_opand)) return 0;
						double item2= pop_opand(&st_opand);

						if (check_empty_opands(&st_opand)) return 0;
						double item1= pop_opand(&st_opand);

						if (check_empty_opts(&st_optor)) return 0;
						char op= pop_optor(&st_optor);

						if (check_full_opands(&st_opand)) return 0;
						push_opand(&st_opand, result(item1,item2,op));
					}

					if (check_full_opts(&st_optor)) return 0;
					push_optor(&st_optor, str[i]);

				}
			}
		}

		else
		{
			// if some character is found which is not a digit or operator or space etc..(basically invalid exp)
			return 0;
		}

		flag=0;
	}


    /* When parsing of the expression is done then we need to execute for the operators currently in the operator stack*/
	while (st_optor.top!=-1)
	{
		if (check_empty_opands(&st_opand)) return 0;
		double item2= pop_opand(&st_opand);

		if (check_empty_opands(&st_opand)) return 0;
		double item1= pop_opand(&st_opand);

		if (check_empty_opts(&st_optor)) return 0;
		char op= pop_optor(&st_optor);

		if (check_full_opands(&st_opand)) return 0;
		push_opand(&st_opand, result(item1,item2,op));

	}

	/*.....................................................................................................................*/

	if (st_opand.top==0)// Only the answer should be present in the operand stack else error
	{
		sprintf(ans,"%lf",pop_opand(&st_opand));
		return 1;
	}
	else
		return 0;
}








/* ............................................. The main function ...................................................*/

int main(int argc, char** argv)
{
	int port;
	sscanf(argv[1], "%d", &port);// the port taken from command line
	int serv;//socket descriptor for the server
	
	// in=(char *)malloc(500*sizeof(char));
	// out=(char *)malloc(500*sizeof(char));
	

	serv=socket(AF_INET, SOCK_STREAM, 0);
	// AF_NET --> Ipv4 protocol, SOCK_STREAM --> TCP, 0-->IP
	if(serv<=0){
		printf("Error while creating socket in Server");
		exit(0);
	}

	printf("Socket Created!\n");
	struct sockaddr_in serv_addr;

	bzero((char *)&serv_addr,sizeof(serv_addr));// initializes with zero

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;//Kernel chooses the addresses
	serv_addr.sin_port = htons(port);//converts the host byte order to network byte order

	int b = bind(serv, (struct sockaddr *)& serv_addr, sizeof(serv_addr));// binding IP address and port number to create a socket

	if (b<0)
	{
		printf("Binding Error from Server\n");
		exit(0);
	}

	printf("Bind done\n");

	int l = listen(serv,30);// Listening for clients to connect, total 30 pending clients it can accomodate in the queue

	if (l<0)
	{
		printf("Error while listening by the Server\n");
		exit(0);
	}

	printf("Starting to accept\n");

	while (1)
	{
		FILE *fp;
		fp=fopen("input.txt","a+");// opens the predefined file using the file descriptor
		char in[max]={0};
		fseek(fp,0,SEEK_SET);
		int len = sizeof(serv_addr);
		int new_sock = accept(serv, (struct sockaddr *)& serv_addr,  (socklen_t *)&len);// accepts and returns the socket descriptor to which the client communicates with the server

		if (new_sock<0)
		{
			printf("Error while accepting the connection by the server\n");
			exit(0);
		}

		printf("Accepted!\n");

		int r = read(new_sock, in, max); // reads from the client 
		if (r<0)
		{
			printf("Error while reading by the Server\n");
			exit(0);
		}


		char *exp = strtok(in," ");
		/* .........................................If the command is WRITEX from the client................................*/
		if (!strcmp(exp,"WRITEX"))
		{
			exp=strtok(NULL,"\0");
			int req=request(fp,exp);
			if (req==2)
			{
				
				send(new_sock, "Invalid Expression\n" , 30, 0);
			}
			else if (req==1)
			{
				
				send(new_sock, "Expression already present\n" , 40, 0);
			}
			else if (req==0)
			{
				
				send(new_sock, "Success\n" , 20, 0);

			}
			fclose(fp);
		}

		/*........................................If the command is EVALUATEX from the client...............................*/

		else if (!strcmp(exp,"EVALUATEX"))
		{
			exp=strtok(NULL," ");
			char ans[50]={0};
			size_t len=0;
			char *line=NULL;
			int count=1;
			int line_no=atoi(exp);

			//printf("%d\n",line_no );

			while (getline(&line, &len, fp)!=-1)
			{
				if (count==line_no)
				{
					int s=evaluation(line,ans);

					if (!s)
						send(new_sock, "Error\n" , 20, 0);
					else
					{
						double final_ans=strtod(ans,NULL);

						//printf("final %lf\n",final_ans);

						char temp[50];
						sprintf(temp,"%lf",final_ans);
						
						send(new_sock, temp, strlen(temp), 0);
						//printf("YEEEEEEEEEEE\n");
						break;
					}

				}
				count++;
			}

			//printf("count %d \n",count );
			
			if (line_no>count) send(new_sock, "Specified line is absent\n" , 40, 0);
			fclose(fp);
		}
		else
		{
			send(new_sock, "The request cannot be processed 'Wrong message from the client'\n" , max, 0);

			
		}

		

		printf("Client Serviced\n");
		//send(new_sock, out, strlen(out), 0);

		close(new_sock);

	}

	close(serv);
	
	return 0;
}