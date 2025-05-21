#include "define.h" 

//Initializing mutexes
pthread_mutex_t mutex_seats=PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t mutex_waitingtime= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_servicetime= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_telephones= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cashiers= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_screen= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_balance= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_time= PTHREAD_MUTEX_INITIALIZER;

//creating conditions
pthread_cond_t cond_telephones=PTHREAD_COND_INITIALIZER; //by default 
pthread_cond_t cond_cashier=PTHREAD_COND_INITIALIZER;

//initializing variables
int telephones=Ntel;
int cashiers=Ncash;
unsigned int seedp; // value used in rand to get different sequences of nums
int rc; // response code 
int seatsA=Nseat*NzoneA; // counters for available seats in each zone
int seatsB=Nseat*NzoneB;
int currSeat; //current seat
int counter; //seat counter
int reservation;//boolean
int i,j,z;
int cost; //cost per costumer
int total_cost=0; //total cost
int successful_payments=0; 
int successful_reservsA=0;
int successful_reservsB=0;
double balance=0;
char zone;
int seats[300];
double total_waiting_time, tel_waiting_time, cash_waiting_time;

 

void *func( void * id){ //runs everytime a thread is created
	
	int cust_id= *(int *)id;
	//generating all random variables
	int rand_seats= rand_r(&seedp) % Nseathigh+ Nseatlow ; // generating the rand num of seats each costumer 
	int tel_rand_time=rand_r(&seedp) % tseathigh + tseatlow; // random time needed for tel to check availability
	int cash_rand_time=rand_r(&seedp) % tcashhigh + tcashlow; //random time needed for the cashier
	float rand_zone= (rand_r(&seedp) % 100)/100; // if rand_zone<=0.3 the seat belongs to zoneA
	float rand_payment= (rand_r(&seedp) % 100)/100; //if rand_payment<=0.9 the payment was successful

	
	
	
	//wait until tel is available
	while(telephones<1){ 
		pthread_cond_wait(&cond_telephones, &mutex_telephones);
	}
	
	//CALL
	telephones--;
	rc = pthread_mutex_lock(&mutex_telephones);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}

	unsigned int sleep(unsigned int tel_rand_time); //random waiting time to book the seats

	pthread_mutex_t zone_mutex;
	pthread_mutex_t seatsA_mutex;
	pthread_mutex_t seatsB_mutex; 
	
	rc = pthread_mutex_lock(&zone_mutex);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}

	rc = pthread_mutex_lock(&mutex_seats);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}

	
	if(rand_zone<=PzoneA){
		
		currSeat=0; //runner 
		if (seatsA==0){ //no seats available
			printf("%p: Your reservation was cancelled: All seats in zone A are already reserved.\n", id);
			//edw kati paizei me total waiting time
			exit(-1);
		}else{ //checking for availability in zone A
			rc = pthread_mutex_lock(&seatsA_mutex);
			if (rc != 0) {	
				printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
				pthread_exit(&rc);
			}

			for(i=1; i<=NzoneA; i++){ //runs through each row
				counter=0; // counts the num of sequential seats
				//if(counter==rand_seats) break;
				for(j=1; j<=Nseat; j++){//runs through every seat in each row
					currSeat++;
					if(seats[currSeat]!=0){ //0=empty
						counter++;	
					}
					if(counter==rand_seats){
						//reserve the seats
						for(z=currSeat-rand_seats;z<=currSeat;z++){
							seats[z]=1; //1=reserved
						}
						seatsA= seatsA-counter;
						cost=rand_seats*CzoneA;
						total_cost+=cost;
						successful_reservsA++;
						zone='A';
						break;
					}
				}//for j
				if(counter!=rand_seats & i==NzoneA){
					printf("%p: Your reservation was cancelled: There are not enough sequential seats in zone A.\n", id);
					exit(-1);
				}
			}//for i
			rc = pthread_mutex_unlock(&seatsA_mutex);
		}
	}else{//zone B
	
		currSeat=100;
		if(seatsB==0){ //no seats available
			printf("%p: Your reservation was cancelled: All seats in zone B are already reserved.\n", id);
			//edw kati paizei me total waiting time
			exit(-1);										
		}else{ //checking for availability in zone B
			rc = pthread_mutex_lock(&seatsB_mutex);
			if (rc != 0) {	
				printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
				pthread_exit(&rc);
			}

			for(i=11; i<=30; i++){ //each row from 11 to 30

				if (counter==rand_seats) break;
				counter=0;
				
				for(j=1; j<=10; j++){//runs through every seat in each row
					currSeat++;
					if(seats[currSeat]==0){ //0=empty
						counter++;	
					}
					if(counter==rand_seats){
						//reserve the seats
						for(z=currSeat-rand_seats;z<=currSeat;z++){
						
							seats[z]=1; //1=reserved
						}
						seatsB=seatsB-counter;
						cost=rand_seats*CzoneB;
						total_cost+=cost;
						successful_reservsB++;
						zone='B';
						break;
					}
				}//for j
				if(counter!=rand_zone && i==NzoneB){
					printf("%p: Your reservation was cancelled: There are not enough sequential seats in zone B. ", id);
					exit(-1);
				}
			}//for i			
			rc = pthread_mutex_unlock(&seatsB_mutex);
			if (rc != 0) {	
				printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
				pthread_exit(&rc);
			}
		}
	}
	rc = pthread_mutex_unlock(&zone_mutex);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	rc = pthread_mutex_unlock(&mutex_seats);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	rc = pthread_mutex_unlock(&mutex_telephones);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	/////////////////////////pthread_exit(&rc); 
	telephones++;
	
	pthread_cond_signal(&cond_telephones);
	
	rc = pthread_mutex_lock(&mutex_time);
	if (rc != 0) {	
		printf("ERROR: Return code from pthread_mutex_lock() is %d\n",rc);
		pthread_exit(&rc);
	}
	
	total_waiting_time+=tel_waiting_time;
	
	rc = pthread_mutex_unlock(&mutex_time);
	if (rc != 0) {	
		printf("ERROR: Return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	//wait until cashier is available 
	while(cashiers<1){
		pthread_cond_wait(&cond_cashier, &mutex_cashiers);
	}
	
	cashiers--;
	rc = pthread_mutex_lock(&mutex_cashiers);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	sleep(cash_rand_time);
	rc = pthread_mutex_unlock(&mutex_cashiers);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	cashiers++;
	rc = pthread_cond_signal(&cond_cashier);
	
	rc = pthread_mutex_lock(&mutex_time);
	if (rc != 0) {	
		printf("ERROR: Return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	total_waiting_time+=cash_waiting_time;
	
	rc = pthread_mutex_unlock(&mutex_time);
	if (rc != 0) {	
		printf("ERROR: Return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	//PAYMENT
	if(rand_payment<0.1){
		//payment failed
		rc = pthread_mutex_lock(&mutex_seats);
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
		}
		z=currSeat-rand_seats;
		for(z; z<=currSeat; z++){
			seats[z]=0; //0=empty
		}
		rc = pthread_mutex_unlock(&mutex_seats);
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(&rc);
		}
		if(zone=='A'){
			seatsA= seatsA+counter;
			successful_reservsA--;
		}else{
			seatsB= seatsB+counter;
			successful_reservsB--;			
		}
		total_cost-=cost;
		exit(-1);
	}else{
		//successful payment
		successful_payments++;
		rc = pthread_mutex_lock(&mutex_balance);
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
		}	
		balance+=cost;
		rc = pthread_mutex_unlock(&mutex_balance);
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(&rc);
		}
		rc = pthread_mutex_lock(&mutex_screen);
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
		}

		/////////////printf("%d: Your reservation is complete.\n Your seats are in zone %c, row %d seats:.\n The cost of your transaction is: %d",cust_id, zone, i,??, cost);
		rc = pthread_mutex_unlock(&mutex_screen);
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(&rc);
		}
	}
	
}//func

int main(int argc, char *argv[]){

	if (argc != 3) {
		printf("ERROR: the program should take two arguments, the number of threads to create and the seed for the generator.\n");
		exit(-1);
	}

	int Ncust = atoi(argv[1]); // 1st argument - number of costumers
	int seedp = atoi(argv[2]); // 2nd argument - seed

	// checking argument
	if (Ncust <= 0) {
		printf("ERROR: the number of costumers should be a positive number.");
		exit(-1);
	}

	// mutex initialize
	rc = pthread_mutex_init(&mutex_seats, NULL);
	if (rc != 0) {
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}

	rc = pthread_mutex_init(&mutex_waitingtime, NULL);
	if (rc != 0) {
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}

	rc = pthread_mutex_init(&mutex_servicetime, NULL);
	if (rc != 0) {
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}

	rc = pthread_mutex_init(&mutex_telephones, NULL);
	if (rc != 0) {
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}

	rc = pthread_mutex_init(&mutex_cashiers, NULL);
	if (rc != 0) {
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}

	rc = pthread_mutex_init(&mutex_screen, NULL);
	if (rc != 0) {
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}

	rc = pthread_mutex_init(&mutex_balance, NULL);
	if (rc != 0) {
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}

	// condition initialize
	rc = pthread_cond_init(&cond_telephones, NULL);
	if (rc != 0) {
		printf("ERROR: return code from pthread_cond_init() is %d\n", rc);
		exit(-1);
	}

	rc = pthread_cond_init(&cond_cashier, NULL);
	if (rc != 0) {
		printf("ERROR: return code from pthread_cond_init() is %d\n", rc);
		exit(-1);
	}


	//creating threads
	////////logika prepei na uparxei kai kapoio arxiko nhma to opoio dhmiourgei ta nhmata kai stop telos tha tupwsei kai ta apotelesmata
	pthread_t * threads;

	threads = malloc(Ncust * sizeof(pthread_t)); //allocates the specified num of bytes
	if (threads == NULL) {
		printf("NOT ENOUGH MEMORY!\n");
		return -1;
	}

	//initializing seats array
	int seats[300];
	for(i=1; i<300; i++){
		seats[i]=0;
	}
	
	pthread_t * threadId;
	threadId= malloc(Ncust * sizeof(pthread_t)); //allocates the specified num of bytes
	int cust_id[Ncust];

	// creating the threads
	for(i =0; i<Ncust; i++){

		cust_id[i] = i; // array in which customers id's are created

		rc = pthread_create(&threadId[i], NULL, &func, &cust_id[i]);
		// elegxos swsths dhmioyrgias threads
		if (rc != 0) {
		printf("ERROR: return code from pthread_create() is %d\n", rc);
		exit(-1);
		}
	}


	//waits for threads to terminate
	for(i=0; i<Ncust; i++){
		rc = pthread_join(threadId[i], NULL);

		if (rc != 0) {
		printf("ERROR: return code from pthread_join() is %d\n", rc);
		exit(-1);
			}
	}

	//free memory alloc
	free(threads);
	free(threadId);
	


	// destroying mutexes
	rc = pthread_mutex_destroy(&mutex_seats);
	if (rc != 0) {
	printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
	exit(-1);
	}

	rc = pthread_mutex_destroy(&mutex_waitingtime);
	if (rc != 0) {
	printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
	exit(-1);
	}

	rc = pthread_mutex_destroy(&mutex_servicetime);
	if (rc != 0) {
	printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
	exit(-1);
	}

	rc = pthread_mutex_destroy(&mutex_telephones);
	if (rc != 0) {
	printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
	exit(-1);
	}

	rc = pthread_mutex_destroy(&mutex_cashiers);
	if (rc != 0) {
	printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
	exit(-1);
	}

	rc = pthread_mutex_destroy(&mutex_screen);
	if (rc != 0) {
	printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
	exit(-1);
	}

	rc = pthread_mutex_destroy(&mutex_balance);
	if (rc != 0) {
	printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
	exit(-1);
	}

	// destroying conditions
	rc = pthread_cond_destroy(&cond_telephones);
	if (rc != 0) {
	printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc);
	exit(-1);
	}

	rc = pthread_cond_destroy(&cond_cashier);
	if (rc != 0) {
	printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc);
	exit(-1);
	}

	return 0;
}