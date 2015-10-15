#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define IN "waehrung.dat"
#define OUT "liste.dat"
#define ANZAHL 8
#define Y 4
#define EURODOLLAR 1.0885
#define EUROKRONE 27.029


bool eingabe ( double betraege [], int anzb, char waehrung [] [Y], int anzw);
bool waehrung2double (double betraege [], int anzb, char waehrung [][Y], int anzw, char zeile[], int i);
int ausgabe (double betraege [], int anzb, char waehrung [][Y],int anzw);



int main ( void )
{

	int anzb = ANZAHL;
	int anzw = ANZAHL;
	double betraege [ANZAHL]= {};
	char  waehrung [ANZAHL] [Y] = {{}};
	eingabe (betraege, anzb, waehrung, anzw);
	ausgabe (betraege , anzb, waehrung,anzw);





	return 0;

}

bool eingabe ( double betraege [], int anzb, char waehrung [][Y] , int anzw)
{
  	FILE * fpi1 = NULL  ;
	char zeile [ 42] = "" ;
	int i = 0, s=0,j=0;

	if ( NULL == ( fpi1 = fopen ( IN , "r" ) ) )
	{
		fprintf ( stderr , "Datei %s kann nicht zum lesen "
												"geöffnet werden." , IN ) ;
		return false;
	}


	while ( NULL !=  fgets ( zeile , sizeof ( zeile ) - 1 , fpi1 ) && ++ i )  //führt wahrung2double durch bis alle Elemente erfasst wurden
	{
	waehrung2double (betraege, anzb, waehrung, anzw, zeile, i);

	}
	fclose (fpi1);
	for(i=0; i<anzb; i++)
	{
	if (betraege[i]==0.0)         // entfernt Beträge die auf Null gesetzt wurden
		{

		for(s=i; s<anzb; s++)
		{

			for (j=0;j<Y; j++)
			{

				waehrung[s][j]=waehrung[s+1][j];
			}

			betraege[s]=betraege[s+1];

			}
			anzb=anzb-1;
			anzw=anzw-1;
			i=i-1;
		}
	}

	for (i=0; i<anzb; i++)
	{
	if (*(waehrung[i])!=-128 &&*(waehrung[i])!='$'&&(waehrung[i][0]!='C'||waehrung[i][1]!='Z'||waehrung[i][2]!='K'))  // -128 ist hier auf diesem System das Eurozeichen!!!
	{
		for(s=i; s<anzb; s++)                              //entfernt ungültige Währungen
		{
			for (j=0;j<Y; j++)
			{
				waehrung[s][j]=waehrung[s+1][j];
			}


			betraege[s]=betraege[s+1];
			i=i-1;

		}
		anzb=anzb-1;
		anzw=anzw-1;
	}
	}
	if (anzb==0||(anzw!=anzb))
	{
		return false;
	}
	return true;
}


bool waehrung2double (double betraege[], int anzb, char waehrung[][Y], int anzw, char zeile[], int i)
{

	int s=0;
	int o=0;
	int k=0;
	char* speicher1 ="";
	char* speicher2 ="";
	char* Abschnitt =strtok(zeile," ");                     //spaltet die Zeile am Leerzeichen und erzeugt 2 Puffer
	if (Abschnitt)
	{
		speicher1=Abschnitt;
		o++;
		if ( (Abschnitt  = strtok(NULL, " ")) )
		{
				speicher2=Abschnitt;
		}
	}


	if ((atof (speicher1)==0.0))
		{   int c= strlen (speicher2);

		for (s=c; s>=0; s--)
			{
				if(*(speicher2+s)==','&&(*(speicher2+s+1)!='\0'&&*(speicher2+s+2)!='\0'&&*(speicher2+s+5)=='\0'))  // erzeugt Dezimalpunkt statt Dezimalkomma
				{
					*(speicher2+s)='.';
				}
			}

				for(s=0;s<=c;s++)
				{
					if(*(speicher2+s)=='.'&&*(speicher2+s+1)!='\0'&&*(speicher2+s+2)!='\0'&&*(speicher2+s+3)!='\0'&&*(speicher2+s+4)=='.')  //entfernt Tausendertrennzeichen
				    {
						for(k=s; k<=c&&*(speicher2+k)!='\0'; k++)
						{
							*(speicher2+k)=(*(speicher2+k+1));

						}

					}
				}
				for(s=0;s<=c;s++)
				{
					if(*(speicher2+s)==','&&*(speicher2+s+1)!='\0'&&*(speicher2+s+2)!='\0'&&*(speicher2+s+3)!='\0'&&*(speicher2+s+4)=='.')
					{
						for(k=s; k<=c&&*(speicher2+k)!='\0'; k++)
						{
								*(speicher2+k)=(*(speicher2+k+1));

						}

					}
				}

				for(s=0; s<c; s++)
				{
					if(*(speicher2+s)=='.'&&((*(speicher2+s+1)==','||*(speicher2+s+2)==','||*(speicher2+s+3)==',')||      //setzt Beträge auf Null, die nicht interpretierbar sind (mehr als 2 Nachkommastellen, Abstand zwischen Tausendertrennzeichen !=3)
						(*(speicher2+s+1)=='.'||*(speicher2+s+2)=='.'||*(speicher2+s+3)=='.')))
					{
						*(speicher2)='\0';return false;
					}
					if(*(speicher2+s)==','&&((*(speicher2+s+1)==','||*(speicher2+s+2)==','||*(speicher2+s+3)==',')||
															(*(speicher2+s+1)=='.'||*(speicher2+s+2)=='.'||*(speicher2+s+3)=='.')))
					{
						*(speicher2)='\0';return false;
					}
				}
				for (s=c; s>=0; s--)                  // entfernt Beträge, die ein Komma enthalten, das nicht ersetzt wurde (und somit an einem fehlerhaften Platz waren)
				{
					if(*(speicher2+s)==',')
					{
						*(speicher2)='\0';return false;
					}
				}


			betraege[i-1]= (double) atof(speicher2);                  //speichert Beträge bzw Währung in ihre Arrays
			for(o=0; o<Y; o++)
			{
				waehrung[i-1][o]=*(speicher1+o);

			}

		}



	else if ((atof (speicher2))==0.0)   //Kommentare siehe oben
	{
			int c =strlen(speicher1);
		 	int s=0;
		 	int k=0;
			for (s=c; s>=0; s--)
			{
				if(*(speicher1+s)==','&&(*(speicher1+s+1)!='\0'&&*(speicher1+s+2)!='\0'&&(*(speicher1+s+3)=='\0'||*(speicher1+s+4)=='\0'||*(speicher1+s+5)=='\0')))
				{
					*(speicher1+s)='.';
				}
			}

			for(s=0;s<=c;s++)
			{
				if(*(speicher1+s)=='.'&&*(speicher1+s+1)!='\0'&&*(speicher1+s+2)!='\0'&&*(speicher1+s+3)!='\0'&&*(speicher1+s+4)=='.')
				{
						for(k=s; k<=c&&*(speicher1+k)!='\0'; k++)
						{
							*(speicher1+k)=(*(speicher1+k+1));

						}

				}
			}
			for(s=0;s<=c;s++)
			{
					if(*(speicher1+s)==','&&*(speicher1+s+1)!='\0'&&*(speicher1+s+2)!='\0'&&*(speicher1+s+3)!='\0'&&*(speicher1+s+4)=='.')
					{

						for(k=s; k<=c&&*(speicher1+k)!='\0'; k++)
						{
								*(speicher1+k)=(*(speicher1+k+1));

						}

					}
			}

			for(s=0; s<c; s++)
			{
					if(*(speicher1+s)=='.'&&((*(speicher1+s+1)==','||*(speicher1+s+2)==','||*(speicher1+s+3)==',')||
						(*(speicher1+s+1)=='.'||*(speicher1+s+2)=='.'||*(speicher1+s+3)=='.')))
					{
						*(speicher1)='\0';return false;
					}
					if(*(speicher1+s)==','&&((*(speicher1+s+1)==','||*(speicher1+s+2)==','||*(speicher1+s+3)==',')||
															(*(speicher1+s+1)=='.'||*(speicher1+s+2)=='.'||*(speicher1+s+3)=='.')))
					{
						*(speicher1)='\0';return false;
					}
			}
			for (s=0; s<=c; s++)
			{
					if(*(speicher1+s)==',')
					{
						*(speicher1)='\0';return false;
					}
			}
			betraege[i-1]= (double) atof(speicher1);
			for(o=0; o<Y; o++)
			{
				waehrung[i-1][o]=*(speicher2+o);

			}
		}
	return true;
}

int ausgabe (double betraege [], int anzb, char waehrung [][Y],int anzw)
{
	FILE * fpo = NULL;
	int i=0;
	if ( NULL == ( fpo = fopen ( OUT , "a" ) ) )
	{
		fprintf ( stderr , "Datei %s kann nicht zum Anhängen geöffnet werden\n" , OUT ) ;
		return -1 ;
	}
	if (ftell(fpo)==0)                     // Druckt einen Kopf, falls Position in Datei==0
	{
		fprintf(fpo,"        €        |        $        |       CZK       \n");
	}
	for (i=0; i<anzb;i++)
	{
		if(*(waehrung[i])==(-128))
		{
			fprintf(fpo, "%17.2lf|%17.2lf|%17.2lf\n",betraege[i], betraege[i]*EURODOLLAR, betraege[i]*EUROKRONE);
		}
		if(*(waehrung[i])=='$')
		{
			fprintf(fpo, "%17.2lf|%17.2lf|%17.2lf\n",betraege[i]/EURODOLLAR, betraege[i], (betraege[i]/EURODOLLAR)*EUROKRONE);
		}
		if((waehrung[i][0]=='C'&&waehrung[i][1]=='Z'&&waehrung[i][2]=='K'))
		{
			fprintf(fpo, "%17.2lf|%17.2lf|%17.2lf\n",betraege[i]/EUROKRONE, (betraege[i]/EUROKRONE)*EURODOLLAR, betraege[i]);
		}
	}
	fclose ( fpo ) ;
	printf("        €        |        $        |       CZK       \n");                 //Ausgabe am Bildschirm
	for (i=0; i<anzb;i++)
	{
		if(*(waehrung[i])==(-128))
		{
			printf("%17.2lf|%17.2lf|%17.2lf\n",betraege[i], betraege[i]*EURODOLLAR, betraege[i]*EUROKRONE);
		}
		if(*(waehrung[i])=='$')
		{
			printf("%17.2lf|%17.2lf|%17.2lf\n",betraege[i]/EURODOLLAR, betraege[i], (betraege[i]/EURODOLLAR)*EUROKRONE);
		}
		if((waehrung[i][0]=='C'&&waehrung[i][1]=='Z'&&waehrung[i][2]=='K'))
		{
			printf("%17.2lf|%17.2lf|%17.2lf\n",betraege[i]/EUROKRONE, (betraege[i]/EUROKRONE)*EURODOLLAR, betraege[i]);
		}
	}
	return 0 ;
}


