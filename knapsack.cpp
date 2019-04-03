#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
       // l [100]: Weight of each object (maximum weight: 1000)
       // x: weight per utility, l / u
       // y: quantity that fits in the backpack
       // L: maximum backpack capacity: 1000
       // C: remaining capacity
       // zmax: maximum utility solution
       // z: maximum branch utility
       // time1: complete enumeration time
       // time2: heuristic time

struct list{
       int u, l, x, y;
};

main(){
    FILE *arquivo;
    struct list objeto[100], aux;
    int quant=0, i, j, L=1000, soma1=0, soma2=0, k=0, parada=0, z=0, zmax=0, cont=0, w , M=0, Mmax=0, exec;
    float tempototal1=0, tempototal2=0;
    clock_t tempo1, tempo2;

    arquivo= fopen ("Saida.txt", "a");//Folder of results
	srand(time(NULL));//Using the clock to set the random seed

	for(w=0;w<6;w++){  //repetition for the 6 possible conditions, being the quantity of variables that will be tested
		system("cls");
		printf("By: Wellington Luiz da Silva Junior\n\n");
		if(w==0){
			quant=20;
		}
		if(w==1){
			quant=30;
		}
		if(w==2){
			quant=50;
		}
		if(w==3){
			quant=70;
		}
		if(w==4){
			quant=80;
		}
		if(w==5){
			quant=100;
		}
		fprintf(arquivo,"By: Wellington Luiz da Silva Junior\n\n");
    	fprintf(arquivo,"\n----------------------------------------------------------------------------");
    	fprintf(arquivo,"\n Variables: %d\n", quant);

		for (exec=0;exec<1000;exec++){

    	fprintf(arquivo,"\n\n Test of %d variables number %d\n", quant, exec+1);
    	printf (" Test of %d variables number %d\n\n" , quant, exec+1);

 	    for (i=0;i<quant;i++){                                      //Generates random values for the backpack

 	    	objeto[i].u= 1+rand()%10;
          	objeto[i].l= 1+rand()%1000;
          	objeto[i].x= objeto[i].l/objeto[i].u;
      	}
      	for (i=0;i<quant-1;i++)                                    //Order the backpack
      	for (j=0;j<quant-1;j++){
        	if (objeto[j].x > objeto[j+1].x){
                aux.u= objeto[j].u;
                aux.l= objeto[j].l;
                aux.x= objeto[j].x;
            	objeto[j].u= objeto[j+1].u;
                objeto[j].l= objeto[j+1].l;
            	objeto[j].x= objeto[j+1].x;
                objeto[j+1].u= aux.u;
                objeto[j+1].l= aux.l;
            	objeto[j+1].x= aux.x;
          }
      }
     //  ************ START OF COMPLETE LISTING ***********

    tempo1= clock();
    k=0;
    do{
    	for (i=k;i<quant;i++){
            if (k != quant)
                objeto[i].y= L/objeto[i].l;                       //quantity of objects that fit in the backpack
              	L= L - (objeto[i].y * objeto[i].l);               //remaining capacity of the backpack
        }

        for (i=0;i<quant;i++)
        	z= z+ objeto[i].y * objeto[i].u;

        if (z>zmax)
        	zmax=z;
          	L=1000;

        for (j=quant-1;j>(-1);j--){                           //take 1 from the last branch to build a
            if (objeto[j].y != 0 && cont != 1){
                objeto[j].y= (objeto[j].y)-1;

    			for (i=0;i<quant;i++)
                	L= L - (objeto[i].y * objeto[i].l);

                k=j+1;
                cont=1;
            }
        }

        cont=0;
        z=0;
        parada=0;

        for (i=0;i<quant;i++){                           //flag of stop
            if (objeto[i].y == 0 && parada == 0)
                parada=0;

			else
        		parada=1;
        }

    }while (parada!=0);

    tempo1= clock()- tempo1;
    tempototal1 = tempototal1 + ((float)tempo1)/CLOCKS_PER_SEC;
    tempo1=0;

    fprintf(arquivo,"\n Enumeracao Completa: %d\n ",zmax);
	printf (" Enumeracao Completa: %d\n", zmax);
	zmax=0;
    z=0;

    //  ************ Heuristics START ***********

    tempo2= clock();                                            //Start the counter
    k=0;
    if (k < (quant - 1))                                          //calculation of the limit for the first branch
    M= soma1+(objeto[k+1].u / objeto[k+1].l) * (L - soma2) + ((objeto[k].l * objeto[k+1].u - objeto[k].u * objeto[k+1].l)/objeto[k+1].l);
    soma1=0;
    soma2=0;

    do{
        for (i=k;i<quant;i++){
            if (k != quant)
                objeto[i].y= L/objeto[i].l;                    //quantity of objects that fit in the backpack
              	L= L - (objeto[i].y * objeto[i].l);               //remaining capacity of the backpack
        }

        for (i=0;i<=k;i++){                                    //Calculates sums for the limiting
            soma1= soma1+ objeto[i].y * objeto[i].u;
            soma2= soma2+ objeto[i].y * objeto[i].l;
        }

        L=1000;
        for (j=quant-1;j>(-1);j--){                           //take 1 from the last branch to build a
            if (objeto[j].y != 0 && cont != 1){
                objeto[j].y= (objeto[j].y)-1;
                if (k < (quant - 1))                        //calculate the limitation, if different from the last
          			M= soma1+(objeto[k+1].u / objeto[k+1].l) * (L - soma2) + ((objeto[k].l * objeto[k+1].u - objeto[k].u * objeto[k+1].l)/objeto[k+1].l);

                if (M>Mmax)
                    Mmax=M;
                else                                         //If M < Mmax, cut the branch
                    objeto[j].y= 0;

                for (i=0;i<quant;i++)
                	L= L - (objeto[i].y * objeto[i].l);          //calculates the available capacity of the backpack

                k=j+1;
                cont=1;
            }
    	}
        soma1=0;
        soma2=0;
        cont=0;
        parada=0;

        for (i=0;i<quant;i++){                              //flag of stop
            if (objeto[i].y == 0 && parada == 0)
                parada=0;
            else
                parada=1;
        }

      }while (parada!=0);

    tempo2= clock()- tempo2;                                //stop the counter

    tempototal2 = tempototal2 + ((float)tempo2)/CLOCKS_PER_SEC;
    tempo2=0;

    fprintf(arquivo,"Heuristic: %d\n", Mmax);
    printf (" Heuristic: %d\n\n", Mmax);
    Mmax=0;
    M=0;

}
tempototal1= tempototal1/exec;
tempototal2= tempototal2/exec;
fprintf(arquivo,"\n +++++++++++++++++++++++++++++++++++++++++++++");
fprintf(arquivo,"\n Average time Enumeration: %f\n", tempototal1);
  fprintf(arquivo,"\n Heuristic average time: %f\n\n", tempototal2);
fprintf(arquivo,"\n +++++++++++++++++++++++++++++++++++++++++++++\n");
printf ("\n\n Average run time Enumeration: %f\n", tempototal1);
printf ("\n Average Heuristic Execution Time: %f\n\n", tempototal2);
}

fclose(arquivo);
system("pause");
}
