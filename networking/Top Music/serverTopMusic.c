/* cliTcpConc.c - Exemplu de client TCP
   Trimite un nume la server; primeste de la server "Hello nume".
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

// server Top Music - model TCP/IP Copyright pagina cursului 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* portul folosit */
#define PORT 3003

extern int errno;
pid_t newProcess;

int main ()
{

    struct sockaddr_in server;	
    struct sockaddr_in from;

    char userId[50],msg[100], msgCod[2], option[2], msgrasp[100]="", type[25], user[25], restricted[25], user_pw[25], result[10000]="", mesaj[1000]; 
	char melodie[10000], gen[25], name_song[50], nume[25], comentariu[1000], id[25], id_user[25], name_artist[50], lnk[50], description[50];

    int sd,  log_in = 0, out;			
	char cod;

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
    	perror ("[server] Eroare la socket().\n");
    	return errno;	
    }

    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));
 
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);


    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
    	perror ("[server]Eroare la bind().\n");
    	return errno;
    }

    if (listen (sd, 1) == -1)
    {
    	perror ("[server]Eroare la listen().\n");
    	return errno;
    }
    
	// start lucru clienti ...

    while (1)
    {
    	int client, length = sizeof (from);

    	printf ("[server]Asteptam la portul %d...\n",PORT);
    	fflush (stdout);

    	client = accept (sd, (struct sockaddr *) &from, &length);

    	if (client < 0){ perror ("[server]Eroare la accept().\n"); continue;} // eroare accept

    	if ((newProcess = fork()) == -1) { close(client); continue; } // eroare la creare proces copil
        
        else if(newProcess == 0){  // lucrez cu copii <-> clientii

    	close(sd);

		int CONECT = 1;

		while(CONECT==1)
		{
    	/* s-a realizat conexiunea, se astepta mesajul */
    	bzero (msgCod, 2); bzero(user,25); bzero(user_pw,25);
		bzero(msgrasp,100); bzero(msg,100); bzero(type,25);

    	printf ("[server]Asteptam mesajul...\n");
    	fflush (stdout);

		/* citirea mesajului */
		if (read (client, msgCod, 2) <= 0)
		{
			perror ("[server]Eroare la read() de la client.\n");
			close (client);	/* inchidem conexiunea cu clientul */
			exit(0);
			continue;		/* continuam sa ascultam */
		}

		printf ("[server]Mesajul a fost receptionat...%s\n", msgCod);

		switch(msgCod[0]) // Primul meniu(Inainte de a te loga)
		{
	
		case '1': // [inregistrarea userilor]
		{

		bzero(user,25);
		bzero(user_pw,25);

		// Deschidere baza de date...

        int err = 0;
		char *err_msg = 0;
		char *SQL;
		sqlite3 *handlerDB;
		sqlite3_stmt *statement;

		int cmd = sqlite3_open("topMusic.db", &handlerDB);

		if (cmd != SQLITE_OK) {
		printf("Cannot open database: %s\n", sqlite3_errmsg(handlerDB));
		sqlite3_close(handlerDB);
		return -1;
		}

		else printf("DB has been opened\n");
	
		if(read(client, user,25)<=0)
		{

		perror ("[server]Eroare la read() de la client.\n");
		close (client);	/* inchidem conexiunea cu clientul */
		continue;		/* continuam sa ascultam */
		}
			
		printf ("[server]Mesajul a fost receptionat...%s - caut in baza de date\n", user);

		SQL = "SELECT * FROM users WHERE name_user = ?;";

		user[strlen(user)-1]='\0';
	
        sqlite3_prepare_v2(handlerDB, SQL , -1, &statement, 0);
		sqlite3_bind_text(statement,1,user,strlen(user),SQLITE_STATIC);

		if(sqlite3_step(statement)==SQLITE_ROW)
		{
			printf("Nume deja existent!");

			strcat(msgrasp,"0Nume de utilizator deja existent.\n");

			if(write(client, msgrasp, 100 )<=0)
				{
					perror ("[server]Eroare la write() catre client.\n");
					continue;		/* continuam sa ascultam */
				}

			sqlite3_finalize(statement);
			sqlite3_close(handlerDB);

		}
		else
		{	
			sqlite3_finalize(statement);
			printf("Nume disponibil, adaug %s in DB\n", user);
			strcat(msgrasp, "1ok");

			if(write(client, msgrasp, 100 )<=0)
				{
					perror ("[server]Eroare la write() catre client.\n");
					continue;		/* continuam sa ascultam */
				}


			SQL = "INSERT INTO users(name_user,restricted) VALUES(?, 'NO');";

			sqlite3_prepare_v2(handlerDB, SQL , -1, &statement, 0);
		    sqlite3_bind_text(statement,1,user,strlen(user),SQLITE_STATIC);


			if(sqlite3_step(statement)==SQLITE_DONE)
			{
				sqlite3_finalize(statement);

				printf("Am adaugat user-ul\n");

				if(read(client, user_pw, 25) <=0 )
				{
					perror ("[server]Eroare la read() de la client.\n");
					close (client);	/* inchidem conexiunea cu clientul */
					continue;		/* continuam sa ascultam */
				}
				
				user_pw[strlen(user_pw)-1]='\0';

				SQL = "UPDATE users SET password = ? WHERE name_user = ?;";

				sqlite3_prepare_v2(handlerDB, SQL , -1, &statement, 0);
		 		sqlite3_bind_text(statement,1,user_pw,strlen(user_pw),SQLITE_STATIC);
				sqlite3_bind_text(statement,2,user,strlen(user), SQLITE_STATIC);

				if(sqlite3_step(statement)==SQLITE_DONE)
				{
					sqlite3_finalize(statement);

					printf("Parola adaugata.\n");

					if(read(client, type, 25) <=0 )
						{
							perror ("[server]Eroare la read() de la client.\n");
							close (client);	/* inchidem conexiunea cu clientul */
							continue;		/* continuam sa ascultam */
						}

					SQL = "UPDATE users SET type = ? WHERE name_user = ?;";

					sqlite3_prepare_v2(handlerDB, SQL , -1, &statement, 0);
					sqlite3_bind_text(statement,1,type,strlen(type),SQLITE_STATIC);
					sqlite3_bind_text(statement,2,user,strlen(user), SQLITE_STATIC);
					
					if(sqlite3_step(statement)==SQLITE_DONE)
					{
						bzero(msgrasp,100);
						sqlite3_finalize(statement);
						sqlite3_close(handlerDB);
						strcat(msgrasp,"Contul dvs. a fost creat cu succes!!!\n");
						if(write(client, msgrasp, 100 )<=0)
							{
								perror ("[server]Eroare la write() catre client.\n");
								continue;		/* continuam sa ascultam */
							}
						printf("User adaugat complet!\n");
					}
					
				}
				
			}
		}
		
		break;
	}  
		case '2': // [LOG_IN]
	 	{
	  		bzero(user,25);
	 		bzero(user_pw,25);

			char *err_msg = 0;
			char *SQL, *SQL1;
			sqlite3 *handlerDB;
			sqlite3_stmt *statement, *stmt;

			int cmd = sqlite3_open("topMusic.db", &handlerDB);

			if (cmd != SQLITE_OK) {
			
			printf("Cannot open database: %s\n", sqlite3_errmsg(handlerDB));
			sqlite3_close(handlerDB);
			return -1;
			}
			else printf("DB has been opened\n");

			if(read(client, user,25)<=0)
			{
			perror ("[server]Eroare la read() de la client.\n");
			close (client);	/* inchidem conexiunea cu clientul */
		//	exit(0);
			continue;		/* continuam sa ascultam */
			}


	 	printf ("[server]Mesajul a fost receptionat...%s - caut in baza de date\n", user);

	 	SQL = "SELECT * FROM users WHERE name_user = ?;";

		user[strlen(user)-1]='\0';

    	sqlite3_prepare_v2(handlerDB, SQL , -1, &statement, 0);
	 	sqlite3_bind_text(statement,1,user,strlen(user),SQLITE_STATIC);
	
	
	 	if(sqlite3_step(statement)==SQLITE_ROW)
	 	{	
			sqlite3_finalize(statement);
	 		printf ("[server]user-ul %s exista in DB \n", user);

			strcat(msgrasp,"0.ok");

			if(write(client, msgrasp, 100 )<=0)
	 			{
	 			perror ("[server]Eroare la write() catre client.\n");
	 		 	continue;		/* continuam sa ascultam */
	 			}
			
	 		if (read (client, user_pw, 25) <= 0)
			{
				perror ("[server]Eroare la read() de la client.\n");
				close (client);	/* inchidem conexiunea cu clientul */
				continue;		/* continuam sa ascultam */
			}

	
	 	SQL = "SELECT * FROM users WHERE name_user = ? and password=?;";

	 	user_pw[strlen(user_pw)-1]='\0';
	

        sqlite3_prepare_v2(handlerDB, SQL , -1, &statement, 0);
		sqlite3_bind_text(statement,1,user,strlen(user),SQLITE_STATIC);
		sqlite3_bind_text(statement,2,user_pw, strlen(user_pw), SQLITE_STATIC);

	 	if(sqlite3_step(statement) == SQLITE_ROW)	
			{	
				bzero(msgrasp,100);
				strcat(msgrasp,"0.ok");

				if(write(client, msgrasp, 100 )<=0)
	 			{
	 			perror ("[server]Eroare la write() catre client.\n");
	 		 	continue;		/* continuam sa ascultam */
	 			}

				printf("Parola corecta!\n");
				sqlite3_finalize(statement);
				//sqlite3_close(handlerDB);

				SQL="SELECT type, restricted, id_user FROM users WHERE name_user=?;";

				sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
				sqlite3_bind_text(statement,1,user, strlen(user), SQLITE_STATIC);

				if(sqlite3_step(statement)==SQLITE_ROW)
				{
				printf("Iau type-ul\n");

				strcpy(type, sqlite3_column_text(statement,0));
				strcpy(restricted, sqlite3_column_text(statement,1));
				strcpy(id_user, sqlite3_column_text(statement,2));
				printf("%s\n", id_user);

				printf("FIRST TIME: %s", type);
				printf("%s \n", restricted);
				sqlite3_finalize(statement);
			//	sqlite3_close(handlerDB);
				strcpy(msgrasp,type);
				if(write(client, msgrasp, 100 )<=0)
	 			{
	 			perror ("[server]Eroare la write() catre client.\n");
	 		 	continue;		/* continuam sa ascultam */
	 			}

				bzero(msgrasp,100);
				bzero(type,25);
	 			printf ("[server]Date corecte! \n"); 
	 			strcat(msgrasp, "Bine ati venit, "); // LOG IN = OK
	 			strcat(msgrasp, user); strcat(msgrasp, "!");

	 			if(write(client, msgrasp, 100 )<=0)
	 			{
	 			perror ("[server]Eroare la write() catre client.\n");
	 		 	continue;		/* continuam sa ascultam */
	 			}

				log_in=1; // cat sta logat
				while(log_in==1)
				{
				bzero(option,2);
				if(read(client, option,2)<=0)
				{
				perror ("[server]Eroare la read() de la client.\n");
				close (client);	/* inchidem conexiunea cu clientul */
				exit(0);
				continue;		/* continuam sa ascultam */
				}

				printf("optiune primita %c\n", option[0]);
				
				switch(option[0])
                {
				  
				  case '1':
				  {	
			
					bzero(result, 10000);
					bzero(gen,25);

					if(read(client, gen,25)<=0)
					{
					perror ("[server]Eroare la read() de la client.\n");
					close (client);	/* inchidem conexiunea cu clientul */
					continue;		/* continuam sa ascultam */
					}


					printf("Gen: %s", gen);
					gen[strlen(gen)-1]='\0';

					strcat(result,"\n\n--- Nume --- Artist --- Descriere --- Nr. voturi\n\n");

					SQL = "SELECT * FROM melodies WHERE genres LIKE '%'||?||'%' ORDER BY votes DESC;";
					//  printf("%s\n", SQL);
					sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
					sqlite3_bind_text(statement,1,gen, strlen(gen), SQLITE_STATIC);
					

					while(sqlite3_step(statement)==SQLITE_ROW)
					{
					
					for(int param = 1 ; param < 5; ++param)
					{
						strcat(result, sqlite3_column_text(statement,param));
						strcat(result, " ");
					}

					strcat(result, "\n");
					
					}

					if(write(client, result, 10000) <=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
	 		 			continue;		/* continuam sa ascultam */
					}

					sqlite3_finalize(statement);

					break;
				  }

				  case '2':
				  {	
					bzero(result, 10000);

					strcat(result,"\n--- Nume --- Artist --- Descriere --- Gen --- Nr. voturi\n\n");
					  
					SQL="SELECT * FROM melodies ORDER BY votes DESC;";

					sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
					
					while(sqlite3_step(statement)==SQLITE_ROW)
					{
	
						for(int param = 1 ; param < 4; ++param)
							{

								strcat(result, sqlite3_column_text(statement,param));
								//printf("%s",)
								strcat(result, " ");
							}
					

					strcat(result, sqlite3_column_text(statement,7));
					strcat(result, " ");
					strcat(result, sqlite3_column_text(statement,4));
					strcat(result,"\n");
					printf("\n");

					}

				//	printf("%s", result);

					if(write(client, result, 10000) <=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
	 		 			continue;		/* continuam sa ascultam */
					}

					sqlite3_finalize(statement);
					

					break;
				  }

				   case '3':
				  {	
					bzero(result, 10000);

					strcat(result,"\n--- Nume --- Artist --- Descriere --- Gen --- Nr. comentarii\n\n");
					  
					SQL="SELECT * FROM melodies ORDER BY comments DESC;";

					sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
					
					while(sqlite3_step(statement)==SQLITE_ROW)
					{
						
						for(int param = 1 ; param < 4; ++param)
							{

								strcat(result, sqlite3_column_text(statement,param));
								strcat(result, " ");
							}
					

					strcat(result, sqlite3_column_text(statement,7));
					strcat(result, " ");
					strcat(result, sqlite3_column_text(statement,5));
					strcat(result,"\n");
					printf("\n");

					}
				//	printf("%s", result);

					if(write(client, result, 10000) <=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
	 		 			continue;		/* continuam sa ascultam */
					}

					sqlite3_finalize(statement);
					break;
				  }

				case '4':
				  {
					  bzero(name_song,50);
					  bzero(id,25);
					  bzero(result,1000);
					  bzero(userId,50);
				
					int err=1;
					int exista=0;
				 strcat(result,"");
				 if(read(client, name_song,50)<=0)
					{
					perror ("[server]Eroare la read() de la client.\n");
					close (client);	/* inchidem conexiunea cu clientul */
					continue;		/* continuam sa ascultam */
					}

				  name_song[strlen(name_song)-1]='\0';
				  SQL="SELECT id_mel FROM melodies WHERE name_song=?;";
				  sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
				  sqlite3_bind_text(statement,1,name_song, strlen(name_song), SQLITE_STATIC);
				
				  if(sqlite3_step(statement)==SQLITE_ROW)
				  {
					exista = 1;
					strcpy(id,sqlite3_column_text(statement,0));
					printf("%s\n", id);
					sqlite3_finalize(statement);
					

				  SQL="SELECT * FROM comments WHERE id_mel=?;";
				  sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
				  sqlite3_bind_text(statement,1,id, strlen(id), SQLITE_STATIC);
				  
				  while(sqlite3_step(statement) == SQLITE_ROW)
				  {
					strcpy(userId, sqlite3_column_text(statement,3));
					SQL1="SELECT name_user FROM users WHERE id_user=?;";

					sqlite3_prepare_v2(handlerDB, SQL1, -1, &stmt, 0);
				 	sqlite3_bind_text(stmt,1,userId, strlen(userId), SQLITE_STATIC);

					if(sqlite3_step(stmt)==SQLITE_ROW)
					{

					err=0;
					strcat(result,"[");
					strcat(result, sqlite3_column_text(stmt,0));
					strcat(result,"]:");
					strcat(result, sqlite3_column_text(statement,2));
					strcat(result,"\n");
					sqlite3_finalize(stmt);
					}
				  }
				  }
				  sqlite3_finalize(statement);
				  
				  if(err==1)
				  {
					  strcat(result,"Niciun comentariu.\n");
				  }
				  
				  if(exista==0)
				  {
					  strcat(result,"Nu exista aceasta melodie.\n");
				  }

				  if(write(client, result, 10000) <=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
	 		 			continue;		/* continuam sa ascultam */
					}
				  

					  break;
				  }

				case '5':
				{

					bzero(melodie,10000);
					bzero(msgrasp,100);
				
					if(read(client, melodie,10000)<=0)
					{
					perror ("[server]Eroare la read() de la client.\n");
					close (client);	/* inchidem conexiunea cu clientul */
					continue;		/* continuam sa ascultam */
					}

					SQL ="INSERT INTO melodies(name_song, name_artist, description, votes, comments, link, genres) VALUES(?,?,?,0,0,?,?);";

					sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
				
					char *p = strtok(melodie, "_");
					int i = 1;
				
					while(p)
					{
						printf("%s ", p);
						sqlite3_bind_text(statement,i,p, strlen(p), SQLITE_STATIC);
						p=strtok(NULL,"_");
						i++;
					}
					
					if(sqlite3_step(statement)==SQLITE_DONE)
					{
						printf("Song added!\n");
						strcat(msgrasp,"Melodie adaugata cu succes. O puteti vizualiza in top de acum.\n");
					}
					else
						strcat(msgrasp,"Melodia nu a putut fi adaugata!\n");
					sqlite3_finalize(statement);
					//printf("%s", p);


					if(write(client, msgrasp, 100) <=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
	 		 			continue;		/* continuam sa ascultam */
					}
					break;
				}
				  case '6':
				  {
					 
					bzero(name_song,50);
					bzero(msgrasp,100);
					if(read(client, name_song,50)<=0)
						{
						perror ("[server]Eroare la read() de la client.\n");
						close (client);	/* inchidem conexiunea cu clientul */
						continue;		/* continuam sa ascultam */
						}

					int err=0;

					name_song[strlen(name_song)-1]='\0';
					if(strstr(restricted,"no") || strstr(restricted,"NO"))	
						{
							SQL="SELECT name_song FROM melodies WHERE name_song=?;";
							
							sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
							sqlite3_bind_text(statement,1,name_song, strlen(name_song), SQLITE_STATIC);
							
							if(sqlite3_step(statement)!=SQLITE_ROW)
							{
								strcat(msgrasp,"Problema vot. Nume indisponibil!\n");
								err=1;
							}

							sqlite3_finalize(statement);

							if(err==0){
							SQL="UPDATE melodies SET votes=votes+1 WHERE name_song=?;";

							sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
							sqlite3_bind_text(statement,1,name_song, strlen(name_song), SQLITE_STATIC);
							
							if(sqlite3_step(statement)==SQLITE_DONE)
								{
									printf("run OK");
									strcat(msgrasp,"Ati votat aceasta melodie!\n");
									sqlite3_finalize(statement);
								}
						

								}
						}
						else if( strstr(restricted,"yes") || strstr(restricted,"YES"))
						{
							strcat(msgrasp,"Nu aveti drepturi sa votati. Contactati unul dintre administratori.");

						}
					
					
					if(write(client, msgrasp, 100) <=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
	 		 			continue;		/* continuam sa ascultam */
					}

					break;
				  }

				
				  case '7':
				  {
					bzero(name_song,50);
					bzero(msgrasp,100);
					
					bzero(id,25);

					if(read(client, name_song,50)<=0)
					{
					perror ("[server]Eroare la read() de la client.\n");
					close (client);	/* inchidem conexiunea cu clientul */
					continue;		/* continuam sa ascultam */
					}
					
					name_song[strlen(name_song)-1]='\0';
					SQL="SELECT name_song FROM melodies WHERE name_song=?;";
					sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
					sqlite3_bind_text(statement,1,name_song, strlen(name_song), SQLITE_STATIC);

					printf("[%s]\n", name_song);
					if(sqlite3_step(statement)!=SQLITE_ROW)
					{	
						printf("nume aiurea mel. \n");
						sqlite3_finalize(statement);
						strcat(msgrasp,"0Melodia nu exista! Ai grija la numele sau!\n");

					if(write(client,msgrasp, 100)<=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
						continue;		/* continuam sa ascultam */
					}
						
					}
					else 
					{	
						strcat(msgrasp,"!Melodie existenta!\n");
						if(write(client,msgrasp, 100)<=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
						continue;		/* continuam sa ascultam */
					}

						printf("nume bun de mel");
						sqlite3_finalize(statement);
						SQL="SELECT id_mel FROM melodies WHERE name_song=?;";
						sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
						sqlite3_bind_text(statement,1,name_song, strlen(name_song), SQLITE_STATIC);
						if(sqlite3_step(statement)==SQLITE_ROW)
						{
						printf("./id-ul:\n");
					
						strcpy(id, sqlite3_column_text(statement,0));
						sqlite3_finalize(statement);
						}

						printf("%s\n", id);

						printf("Melodie existenta ... urmeaza comm.\n");

						if(read(client, comentariu,1000)<=0)
						{
						perror ("[server]Eroare la read() de la client.\n");
						close (client);	/* inchidem conexiunea cu clientul */
						continue;		/* continuam sa ascultam */
						}

						comentariu[strlen(comentariu)-1]='\0';
						printf("%s\n", comentariu);

						SQL="INSERT INTO comments(id_mel,comentariu, id_user) VALUES (?,?,?);";

						sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
						sqlite3_bind_text(statement,1,id, strlen(id), SQLITE_STATIC);
						sqlite3_bind_text(statement,2,comentariu, strlen(comentariu), SQLITE_STATIC);
						sqlite3_bind_text(statement,3,id_user, strlen(id_user), SQLITE_STATIC);

						if(sqlite3_step(statement)==SQLITE_DONE)
						{
							sqlite3_finalize(statement);

							SQL="UPDATE melodies SET comments=comments+1 WHERE name_song=?;";

							sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
							sqlite3_bind_text(statement,1,name_song, strlen(name_song), SQLITE_STATIC);

							if(sqlite3_step(statement)==SQLITE_DONE)
							{
							sqlite3_finalize(statement);
							printf("comm adaugat\n");
							bzero(msgrasp,100);
							strcat(msgrasp,"Comentariu adaugat cu succes!!\n");
							printf("%s", msgrasp);
							

							}
						}		

						if(write(client,msgrasp, 100)<=0)
							{
								perror ("[server]Eroare la write() catre client.\n");
								continue;		/* continuam sa ascultam */
							}	

					//printf("please!\n");
					}

				  break;
				  }

				   case '8':
				  {	
			
					bzero(result, 10000);
					bzero(name_artist,50);

					if(read(client, name_artist, 50)<=0)
					{
					perror ("[server]Eroare la read() de la client.\n");
					close (client);	/* inchidem conexiunea cu clientul */
					continue;		/* continuam sa ascultam */
					}


					
					name_artist[strlen(name_artist)-1]='\0';

					SQL = "SELECT name_song, description, link, votes FROM melodies WHERE name_artist LIKE '%'||?||'%' ORDER BY votes DESC;";
				
					sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
					sqlite3_bind_text(statement,1,name_artist, strlen(name_artist), SQLITE_STATIC);
					
					strcat(result,"Nume --- Descriere --- link --- Nr. voturi\n");
					while(sqlite3_step(statement)==SQLITE_ROW)
					{
					
					for(int param = 0 ; param < 4; ++param)
					{
						strcat(result, sqlite3_column_text(statement,param));
						strcat(result, "  ");
					}

					strcat(result, "\n");
					
					}

					printf("%s", result);
					if(write(client, result, 10000) <=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
	 		 			continue;		/* continuam sa ascultam */
					}

					sqlite3_finalize(statement);

					break;
				  }


				  case '$':
				  {
					bzero(result,1000);
					SQL="SELECT name_user, restricted FROM users WHERE type LIKE '%normal%';";

					sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
					strcat(result,"\n Nume  Restrictionat");
					strcat(result,"\n-------- --------------\n");
					strcat(result,"\n");
					while(sqlite3_step(statement)== SQLITE_ROW)
					{
						strcat(result, sqlite3_column_text(statement,0));
						strcat(result, " ");
						strcat(result, sqlite3_column_text(statement,1));
						strcat(result,"\n");
					}
					sqlite3_finalize(statement);

					if(write(client,result, 10000)<=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
	 		 			continue;		/* continuam sa ascultam */
					}
					break;

				  }
				
				 case '^':
				  {
					bzero(result,1000);
					SQL="SELECT name_user  FROM users WHERE type LIKE '%admin%';";

					sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
					strcat(result,"\nNume administrator");
					strcat(result,"\n----------------------\n");
					strcat(result,"\n");
					while(sqlite3_step(statement)== SQLITE_ROW)
					{
						strcat(result, sqlite3_column_text(statement,0));		
						strcat(result,"\n");
					}
					sqlite3_finalize(statement);

					if(write(client,result, 10000)<=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
	 		 			continue;		/* continuam sa ascultam */
					}
					break;

				  }
				  case '#':
				  {	
					bzero(msgrasp,100);
					bzero(name_song,50);
					bzero(msg,100);
					bzero(id,25);


					if(read(client, name_song,50)<=0)
					{
					perror ("[server]Eroare la read() de la client.\n");
					close (client);	/* inchidem conexiunea cu clientul */
					continue;		/* continuam sa ascultam */
					}

					name_song[strlen(name_song)-1]='\0';
					SQL="SELECT name_song FROM melodies WHERE name_song=?;";
					sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
					sqlite3_bind_text(statement,1,name_song, strlen(name_song), SQLITE_STATIC);

					printf("[%s]\n", name_song);
					if(sqlite3_step(statement)!=SQLITE_ROW)
					{	
						printf("nume aiurea mel. \n");
						sqlite3_finalize(statement);
						strcat(msgrasp,"0Melodia nu exista! Ai grija la numele sau!\n");

					if(write(client,msgrasp, 100)<=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
						continue;		/* continuam sa ascultam */
					}

					}
					else
					{
						
						strcat(msgrasp,"!Melodie existenta!\n");
						if(write(client,msgrasp, 100)<=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
						continue;		/* continuam sa ascultam */
					}

						printf("nume bun de mel");
						sqlite3_finalize(statement);
						SQL="SELECT id_mel FROM melodies WHERE name_song=?;";
						sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
						sqlite3_bind_text(statement,1,name_song, strlen(name_song), SQLITE_STATIC);
						if(sqlite3_step(statement)==SQLITE_ROW)
						{
						printf("./id-ul:\n");
					
						strcpy(id, sqlite3_column_text(statement,0));
						sqlite3_finalize(statement);
						}

						printf("[%s]\n", id);

						printf("Melodie existenta ... urmeaza stergerea sa.\n");

						SQL="DELETE FROM melodies WHERE id_mel=?;";

						sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
						sqlite3_bind_text(statement,1,id, strlen(id), SQLITE_STATIC);

						int rc=sqlite3_step(statement);
						printf("%d",rc);
						if(rc==SQLITE_DONE)
						{
							sqlite3_finalize(statement);
							printf("Stearsa din melodies\n");

							printf("Melodie stearsa total!!!\n");
							strcat(msg,"Melodia a fost stearsa cu succes!\n");

							SQL="DELETE FROM comments WHERE id_mel=?;";

							sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
							sqlite3_bind_text(statement,1,id, strlen(id), SQLITE_STATIC);

							if(sqlite3_step(statement)==SQLITE_DONE)
							{
								sqlite3_finalize(statement);

							}
						}

						if(write(client,msg, 100)<=0)
							{
								perror ("[server]Eroare la write() catre client.\n");
								continue;		/* continuam sa ascultam */
							}

					}
					
					break;
				  }


				  case '~':
				  {
					int err=1;
					bzero(msgrasp,100);
					if(read(client, nume,25)<=0)
					{
					perror ("[server]Eroare la read() de la client.\n");
					close (client);	/* inchidem conexiunea cu clientul */
					continue;		/* continuam sa ascultam */
					}

					nume[strlen(nume)-1]='\0';
					SQL="UPDATE users SET restricted='NO' WHERE name_user=? and type LIKE '%normal%';";
					printf("[%s]", nume);
					sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
					sqlite3_bind_text(statement,1,nume, strlen(nume), SQLITE_STATIC);
					
					if(sqlite3_step(statement)==SQLITE_DONE)
						{
							printf("deblock OK\n"); err=0;
							strcat(msgrasp,"Ati deblocat user-ul: "); err=0;
							strcat(msgrasp, nume); strcat(msgrasp,"\n");
							sqlite3_finalize(statement);
						}
					
					if(err==1)
					{
						strcat(msgrasp,"Nu ati putut debloca. Username incorect!\n");
					}

					sqlite3_finalize(statement);

					if(write(client,msgrasp, 100)<=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
	 		 			continue;		/* continuam sa ascultam */
					}

					  break;
				  }



				  case '@':
				  {
					int err=1;
					bzero(msgrasp,100);
					if(read(client, nume,25)<=0)
					{
					perror ("[server]Eroare la read() de la client.\n");
					close (client);	/* inchidem conexiunea cu clientul */
					continue;		/* continuam sa ascultam */
					}

					nume[strlen(nume)-1]='\0';
					SQL="UPDATE users SET restricted='YES' WHERE name_user=? and type LIKE '%normal%';";
					printf("[%s]", nume);
					sqlite3_prepare_v2(handlerDB, SQL, -1, &statement, 0);
					sqlite3_bind_text(statement,1,nume, strlen(nume), SQLITE_STATIC);
					
					if(sqlite3_step(statement)==SQLITE_DONE)
						{
							printf("block OK\n"); err=0;
							strcat(msgrasp,"Ati blocat user-ul: "); err=0;
							strcat(msgrasp, nume); strcat(msgrasp,"\n");
							sqlite3_finalize(statement);
						}
					
					if(err==1)
					{
						strcat(msgrasp,"Nu ati putut bloca. Username incorect!\n");
					}

					sqlite3_finalize(statement);

					if(write(client,msgrasp, 100)<=0)
					{
						perror ("[server]Eroare la write() catre client.\n");
	 		 			continue;		/* continuam sa ascultam */
					}

					  break;
				  }

                  case '*':
                  {
                    log_in=0;
                    CONECT=0;
                    break;
                  }
                }
			}	

		}
		
	}

	else
		{
		sqlite3_finalize(statement);
		sqlite3_close(handlerDB);	
		bzero(msgrasp,100);
		printf ("[server]parola incorecta!!! \n");
		strcat(msgrasp, "2Parola incorecta!\n");
		if(write(client, msgrasp, 100 )<=0)
		{
			perror ("[server]Eroare la write() catre client.\n");
			continue;		/* continuam sa ascultam */
		}
		
		}

    }
	else 
	{
		sqlite3_finalize(statement);
		sqlite3_close(handlerDB);
		bzero(msgrasp,100);
		printf ("[server]user-ul %s nu exista in DB \n", user);
		strcat(msgrasp, "1User incorect!\n");
		if(write(client, msgrasp, 100 )<=0)
		{
			perror ("[server]Eroare la write() catre client.\n");
			continue;		/* continuam sa ascultam */
		}
	}
	
	break;
	}
	case '5':
	{
		CONECT=0;
		printf("Iesire\n");	
		break;
	}
		
	}
	}	
	/* am terminat cu acest client, inchidem conexiunea */

	printf("stare client(1logat,0off): %d\n", CONECT);
		
	close (client);
    exit(0);
			
	}

	else if (newProcess > 0) { // parent - wait (without blocking) children and close connection 
	close(client);
	while(waitpid(-1,NULL,WNOHANG));
	continue;

	} 

    }			/* while */
}				/* main */