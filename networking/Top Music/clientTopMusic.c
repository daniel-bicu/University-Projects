/* cliTcpConc.c - Exemplu de client TCP
   Trimite un nume la server; primeste de la server "Hello nume".
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
extern int errno;

#define  true 1
#define  false 0 

int port;
int quit;
int login;
char cod[2], option[2], nume[25], genre[25], msj[100], username[25], password[25], type[25];
char msg[100], result[10000], name_song[50], name_artist[50], lnk[50], description[50], mesaj[1000]="", comentariu[1000];



int main (int argc, char *argv[])
{
  int sd;			
  struct sockaddr_in server;	
  char request_msg[100];		

  if (argc != 3)
  {
    printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
    return -1;
  }

  port = atoi (argv[2]);

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
      perror ("Eroare la socket().\n");
      return errno;
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);
  
 
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

  printf("\n\n-------------------------------------- WELCOME------------------------------------\n");
  printf("-------------> Aici gasesti toate melodiile tale favorite!! Toate intr-un singur loc!!! <-------------\n");
  printf("-----------------------> Nu-i asa ca e minunaaaat!?!?<------------------------\n");
  printf("\n\n    Meniu    \n\n\n");
  
  printf("1. Inregistreaza-te\n2. Intra in cont\n3. Despre aplicatie\n4. Contact\n5. Iesire\n");
  printf("\n\n Viata e prea scurta! Alege sa iti spui parerea indiferent despre ce cred ceilalti!! Decizia este doar a ta ^_^!\n\n");
 


while(quit==false)
  {
   printf(" Apasa tasta din dreptul optiunii dorite:\n");

  scanf("%s", cod);
  printf("\n");

  switch(cod[0]){

    case '1': {

       
      write(sd,cod,2);
      bzero(cod, 2);

      
      printf("Pentru a va inregistra vom avea nevoie de urmatoarele informatii: \n");
      printf("Setati un username: ");

      bzero (username, 25);
      fflush (stdout);
      read (0, username, 25);
      write(sd, username,25);

      read(sd, msg, 100);

      if(msg[0]=='0')
      {
        printf("%s", msg+1);
      }
    else 
    {
      bzero(msg,100);
      printf("Setati o parola: ");
      bzero (password, 25);
      fflush (stdout);
      read (0, password, 25);
      write(sd, password,25);

      printf("tipul contului[normal/admin]: ");
      bzero(type,25);
      fflush(stdout);

      read(0, type,25);
      write(sd, type, 25);

      read(sd, msg, 100);

      printf("%s\n", msg);

    }
      break;
    }

    case '2': {

        
       write(sd,cod,2);
       bzero(cod, 2);

       printf("username: ");
       bzero (username, 25);
       fflush (stdout);
       read (0, username, 25);
       write(sd, username, 25);
      
       read(sd, msg, 100);

       if(msg[0]=='0')
      {
        bzero(msg,100);
       printf("password: ");
       bzero(password,25);
       fflush(stdout);

       read(0, password,25);
       write(sd, password, 25);

       
       //read(sd, type,100);
       read(sd, msg, 100);

      if(msg[0]=='0')
      {
       read(sd, type,100);
      //  printf("Am primit tip: %s", type);
       read(sd, msg, 100);
      //  printf("Am primit mesaj: %s", msg);

      printf("\n%s \n ", msg);

       if(strstr(msg,"Bine ati venit"))
        {

        //    printf("\n\n--------------------------------------------------------------------------\n");

            printf("\nNe bucuram sa te vedem din nou aici! Ce doresti sa faci astazi? \n");
            printf("Selecteaza optiunea aleasa si bucura-te de ceea ce descoperi!!! \n\n");

            if(strstr(type,"admin")){
            printf("---Drepturi Administrator ---\n\n");
            printf("@. Blocheaza drepturile pentru un anumit user.\n");
            printf("~. Deblocheaza drepturile pentru un anumit user.\n");
            printf("#. Sterge o melodie.\n");//*
            printf("$. Printeaza lista cu toti utilizatorii.\n");
            printf("-------------------------------\n\n");
            }

            printf("1. Melodii in functie de gen.\n");
            printf("2. Cele mai bune melodii.\n");
            printf("3. Melodiile cele mai comentate. \n"); 
            printf("4. Afiseaza comentariile pentru o anumita melodie\n"); //*
            printf("5. Adauga o melodie\n"); 
            printf("6. Voteaza o melodie. \n");
            printf("7. Adauga comentariu la o melodie\n"); 
            printf("8. Afiseaza melodiile de la artistul tau preferat\n");
            printf("*. Delogheaza-te.");

            if(strstr(type, "normal"))
            {
            printf("\n\n\n In cazul in care intampini probleme contacteaza unul dintre administratori.\n Lista administratorilor este posibila tastand: ^\n");
            printf("\n\n--------------------------------------------------------------------------\n");
            }
          
            

            login=1;

            while(login==1)
            {
                printf("\n[LoggedIn] Apasa tasta din dreptul optiunii dorite:");
                bzero(option,2);
                bzero(name_song,50); bzero(name_artist,50);
                bzero(lnk,50); bzero(description,50);
                bzero(genre,25); bzero(mesaj,1000);
                fflush(stdout);
            
                scanf("%s", option);
               // printf("optiune loop %s\n", option);
                switch(option[0])
                {

                  case '1': // afisare melodii in functie de gen(ord. dupa voturi)
                  {
                    write(sd, option,2);
                    bzero(option,2);

                   // printf("optiune trimisa %s", option);
                    printf("\nSpuneti genul dorit: ");

                    bzero (genre, 25);
                    fflush (stdout);
                    read (0, genre, 25);
                    write(sd, genre,25);

                    bzero(result,10000);

                    read(sd,result,10000);

                    printf("%s", result);

                    break;
                  }

                  case '2': // afisare melodii in functie de voturi
                  {
                    write(sd, option,2);
                    bzero(option,2);
                    bzero(result,10000);
                    printf("Cele mai bune melodii pana in prezent! \n--- Fii pe faza, nu stii cand cineva voteaza alta melodie, asadar nu pierde topul niciodata, s-ar putea sa fii surprins uneori (^_^)!!\n\n");


                    read(sd, result,10000);

                    printf("%s", result);

                    break;
                  }

                  case '3': // afisare melodii in functie de comm
                  { 
                    write(sd, option,2);
                    bzero(option,2);
                    bzero(result,10000);
                    printf("Cele mai comentate(discutate) melodii pana in prezent!\n\n");
                    read(sd, result,10000);

                    printf("%s", result);
                    break;
                  }

                  case '4':
                  {
                    write(sd, option,2);
                    bzero(option,2);
                    bzero(result,10000);
                    bzero(name_song,50);

                    printf("Numele melodiei pentru a afisa comentariile: ");
                    
                    fflush(stdout);
                    read(0,name_song,50);
                    write(sd,name_song,50);

                    read(sd, result,10000);

                    printf("\n%s", result);

                    break;

                  }
                  case '5': // adaugare melodii
                  {
                    write(sd, option,2);
                    bzero(option,2);
                    bzero(msg,100);
                    bzero(name_song,50);
                    bzero(name_artist,50);
                    bzero(description,50);
                    bzero(lnk,50);
                    bzero(genre,25);

                    printf("Numele melodiei:\n");
                    fflush(stdout);
                    read(0,name_song,50); strncat(mesaj,name_song,strlen(name_song)-1); strcat(mesaj,"_");
                    
                    printf("Numele artistului:\n");
                    fflush(stdout);
                    read(0,name_artist,50); strncat(mesaj,name_artist,strlen(name_artist)-1); strcat(mesaj,"_");
                    
                    printf("Descriere:\n");
                    fflush(stdout);
                    read(0,description,50); strncat(mesaj,description,strlen(description)-1); strcat(mesaj,"_");

                    printf("Link:\n");
                    fflush(stdout);
                    read(0,lnk,50); strncat(mesaj,lnk,strlen(lnk)-1); strcat(mesaj,"_");

                    printf("Gen:\n");
                    fflush(stdout);
                    read(0,genre,50); strncat(mesaj,genre,strlen(genre)-1); 

                   // printf("%s", mesaj);
                    write(sd,mesaj,10000);

                    read(sd, msg,100);

                    printf("%s", msg);

                    break;

                  }
        
                  case '6':
                  {
                    write(sd, option,2);
                    bzero(option,2);

                    printf("Numele melodiei pe care o votati: ");
                    bzero(name_artist,50);
                    fflush(stdout);
                    read(0,name_artist,50);
                    write(sd,name_artist,50);

                    read(sd,msg,100);
                    printf("%s", msg);
                    break;
                  }

                  case '7':
                  {
                    write(sd, option,2);
                    bzero(option,2);
                    bzero(msg,100);
                    bzero(comentariu,1000);

                    printf("Adauga comentariu la melodia cu numele:");

                    fflush(stdout);
                    read(0,name_song,50);
                    write(sd,name_song,50);

                    read(sd,msg,100);

                    if(msg[0]=='0')
                    {
                      printf("%s\n", msg+1);
                    }
                    else
                    {
                     printf("Srieti comentariul:\n");
                     fflush(stdout);
                     read(0, comentariu,1000);
                     write(sd,comentariu,1000);
                     //printf("%s\n", comentariu);

                    bzero(msg,100);
                    read(sd,msg,100);

                    printf("%s", msg);
                    }
                    break;

                  }

                  case '8': // afisare melodii in functie de gen(ord. dupa voturi)
                  {
                    write(sd, option,2);
                    bzero(option,2);
                    

                   // printf("optiune trimisa %s", option);
                    printf("\nSpuneti artistul preferat: ");

                    bzero (name_artist, 50);
                    fflush (stdout);
                    read (0, name_artist, 50);
                    write(sd, name_artist,50);

                    bzero(result,10000);

                    read(sd,result,10000);

                    printf("%s", result);

                    break;
                  }

                  case '#':
                  {
                    if(strstr(type, "admin"))
                    {
                    write(sd, option,2);
                    bzero(option,2);

                    bzero(msg,100);
                    bzero(name_song,50);
                   // bzero(mesaj,1000);
                    bzero(msj, 100);

                    printf("Scrieti numele melodiei pe care doriti sa o stergeti: \n");
                    fflush(stdout);

                    read(0, name_song,50);
                    write(sd, name_song,50);

                    read(sd,msg,100);

                    if(msg[0]=='0')
                    {
                      printf("%s\n", msg+1);
                    }
                    else
                    {

                   read(sd,msj,100);

                   printf("%s", msj);

                    }
                    }
                    else
                    printf("Reintrodu optiunea. Asigura-te ca tastezi un numar de la 1-8!\n");
                //    printf("%d", login);

                    break;
                  }


                   case '~': 
                  {
                   

                    if(strstr(type, "admin"))
                    {
                        write(sd, option,2);
                        bzero(option,2);

                        printf("Dati username-ul pentru a il debloca: ");
                        bzero(nume,25);
                        fflush(stdout);
                        read(0,nume,25);
                        write(sd,nume,25);

                        read(sd,msg,100);
                        printf("%s", msg);

                    }
                    else
                    printf("Reintrodu optiunea. Asigura-te ca tastezi un numar de la 1-8!\n");
                    break;  
                  }
                  case '@': 
                  {
                   

                    if(strstr(type, "admin"))
                    {
                        write(sd, option,2);
                        bzero(option,2);

                        printf("Dati username-ul pentru a il bloca: ");
                        bzero(nume,25);
                        fflush(stdout);
                        read(0,nume,25);
                        write(sd,nume,25);

                        read(sd,msg,100);
                        printf("%s", msg);

                    }
                    else
                    printf("Reintrodu optiunea. Asigura-te ca tastezi un numar de la 1-8!\n");
                    break;  
                  }

                  case '$':
                  {
                    if(strstr(type,"admin"))
                      {
                        
                      
                              write(sd, option,2);
                              bzero(option,2);

                              printf("Lista utilizatorilor:\n");

                              bzero(result,10000);
                              read(sd, result,10000);

                              printf("%s", result);
                      }
                      else
                      printf("Reintrodu optiunea. Asigura-te ca tastezi un numar de la 1-8!\n");
                  break;

                  }

                  case '^':
                  {
                    if(strstr(type,"normal"))
                      {
                        
                      
                              write(sd, option,2);
                              bzero(option,2);

                              printf("Lista administratorilor:\n");

                              bzero(result,10000);
                              read(sd, result,10000);

                              printf("%s", result);
                      }
                      else
                      printf("Reintrodu optiunea. Asigura-te ca tastezi un numar de la 1-8!\n");

                    break;
                  }

                  case '*':
                  {
                    
                    write(sd, option,2);
                    bzero(option,2);
                    login=0;
                    quit=true;
                    printf("Delogare ...\n");
                    break;
                  }

                  default:
                  {
                    printf("Reintrodu optiunea. Asigura-te ca tastezi un numar de la 1-8!\n");
                    break;
                  }
                }


            }
               

        
      }
      }
      else if(msg[0]=='2')
      {
            printf("%s\n", msg+1);
      }
   }
      else if(msg[0]=='1')
      {
            printf("%s\n", msg+1);

      }
      break;
      }

    case '3': {
     

      printf("  Aplicatia Top music este o aplicatie de tipul client-server ce are ca scop managementul unui top muzical.\n");
      printf("  Inregistrati-va si platforma va pune la dispozitie sansa de a vota/ comenta sau de a vizualiza topurile muzicale favorite, astfel puteti sa gasiti noi melodii sau puteti impartasii cu alti clienti melodiile dvs. preferate.\n");
      printf("  Mai multe detalii puteti obtine intreband administratorii nostrii pe care ii veti cunoaste in momentul logarii.\n\n Timp petrecut cu placere va dorim!\n\n");
      break;
    }

    case '4':{
      
      printf(" Persoana de contact: Daniel Bicu\n email: daniel.bicu008@yahoo.com\n telefon: 0748 615 282\n Multumim pentru intelegere! \n\n");
      break;
    }

    case '5':{
      
      write(sd,cod,2);
      bzero(cod, 2);
      printf(" O zi frumoasa !\n");
      sleep(2);
      quit=1;
     // exit(0);
      break;
    }

    default:{
    printf("Reintrodu optiunea. Asigura-te ca tastezi un numar de la 1-5!\n");
    break;
    }

  }
}
  /* inchidem conexiunea, am terminat */
  close (sd);
}