#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define num_filozof 8 //filozof sayısını tanımladım
#define num_table 10 //table sayısını tanımladım

void* yeme_dusunme(void *); //yeme-düşünme döngüsü için bir fonksiyon imzası
int yeme_kontrol(); //yeme kontrolunu gerçekleştireceğim fonksiyon imzası

struct Philosopher //Filozof özelliklerini toplu saklamak için filozofun struct yapısını oluşturdum
{
	int id;
	int solCatal;
	int sagCatal;
	int eatTimes;	
	pthread_t thread;
};

struct Table //her bir filozofun sag-sol çatal durumunu bakarak yemelerini kontrol ettim. 
{
	sem_t mutex;  //Semafor kitlenme objesi oluşturdum.
};


struct Table* table; //Fonksiyonda ve mainde kullandığımdan dolayı global bir table tanımladım.
sem_t lock; //Kilit mekanizmalarını sağlamak amacıyla bir lock yani kitlenme objesi oluşturdum.
sem_t kilit; //Kilit mekanizmalarını sağlamak amacıyla kitlenme objesi oluşturdum.
int yemeyen = 0; //Yemeyen kişi sayısını tutmak için bir değişken oluşturdum.
int taz=0; //Tazemeleme sayısını tutmak için bir değişken oluşturdum.
double totalpirinc=2000; // Başlangıctaki toplam pirinc sayısını tanımladım.

int main(int argc, char* argv[])
{	
	double priceTableOpen=99.90; //masa açılma fiyatı tanımlama
	double priceTableReorganize=19.90; //masa tazeleme-yenileme fiyatı
	double rice=20; //Pilavın pirinç kilogramı
	int pilavSayi=1; //pilav baslangıcta 1 adet verilir tazelenme durumda arttırılır 
	double pirinc=100; //Her bir filozofun bir yemede yediğini pirinç miktarı
	double totalpirinc=2000; //Bir pilavdaki pirinc gramı
	double price; //Ücreti hesaplamak için tanımladığım değişken.
	int tazeleme; //Tazeleme sayısını bulmak için tanımadığım değişken
	int i,j; //Döngülerde kullanmak için değişkenler
	int groupNum;	//Bir seferde oluşacak olan grup sayısı
	printf("Please write a group number\n"); 
	scanf("%d",&groupNum);//kullanıcıdan groupNum aldım
	
	int capacity=groupNum*num_filozof; //filozof sayısı ve toplam grup numarasını alarak toplam kapasiteyi buldum.

	if(groupNum<num_table){ //Alınan grup sayısı toplam kapasitedeki table sayısından az olursa 
	
		struct Philosopher* filozoflar = (struct Philosopher*) malloc(sizeof(struct Philosopher) * capacity); //kapasite kadar fizoloflar için yer ayırdım.
		table = (struct Table*)malloc(sizeof(struct Table) * capacity); //Yeme durumunu kontrolu için kapasite kadar bellekte yer ayırdım.
		
		
		sem_init(&lock,0,capacity); //restaurant kapasitesi dolduğunda sinyal göndermek için semafor nesnesi ayarladım.
		
		
		yemeyen = capacity; //yemeyenlerin sayısına başlangıçta tüm kapasiteyi atadım
		for(i=0; i<capacity; i++){//toplam filozof sayısı kadar döndüm
		
	        	sem_init(&table[i].mutex,0,1); // Semafor nesnesini ayarladım. Bir kişi yediğinde forkun içerisinde bulunan mutex değerine sinyal gönderir.
			filozoflar[i].id = i + 1; //Fizolof id'leri 1 den başlayacağı için filozofların idsine i+1 vererek devam ettirdim
			filozoflar[i].eatTimes = 0; //filozofların başlangısta yeme sayısı 0 dır	
			filozoflar[i].sagCatal = i; //Filozofların yemek durumunu için her bir filozofun sağ çatalına i yi atadım

			if (i+1!=capacity){ //kapasite dolu değilse
				filozoflar[i].solCatal = i+1; //filozof sagCatal dan sonra hemen solCatal ile yer. 
			}else{ //kapasite doluysa
				filozoflar[i].solCatal = 0; // filozof yiyemez
			}
			pthread_create(&filozoflar[i].thread, NULL, yeme_dusunme, &filozoflar[i]); //Her bir filozof için bir thead oluşturdum.
	 	
		}

		for(i=0;i<capacity;i++){ //Tüm filozofları döndüm
	     		pthread_join(filozoflar[i].thread, NULL); //join yazarak her bir threadin sonlanmasını bekledim
		}


		printf("\n ---- Hesap ----\n");

		price+=priceTableOpen; //baslangıcta masa açma parası ücrete eklenir
		price+=(2*rice); //baslangıcta olusturulan pirinc miktarı kilogram fiyatı cinsinden price a eklenir
		int count=8;//Bir masadaki filozof sayısı 8 olabileceğinden count değişkenine 8 atadım.
		int masaNum=1; //Masa numarasını tutmak için değişken tanımladım
		for(i=0;i<capacity;i++){ //Toplam kapasite kadar fizoloflar dönülür
			count--; //Masaya biri oturduğunda masadaki kapasite azalır.
			if(totalpirinc>100){ //Toplam pirinç miktarı 100 üzerinde ise 
				printf("Philosopher %d eaten for %d times\n", filozoflar[i].id, filozoflar[i].eatTimes); //Yeme gerçekleşir
				totalpirinc-=(filozoflar[i].eatTimes*100); //Bir fizolofun yediği miktar kadar total pirincten azalma gerçekleşir.
			}
    			
    			if(yemeyen!=0 && totalpirinc<100){ //Yemeyen kisi varsa ve toplam pirinc sayısı 100 altındaysa tazeleme yapılır
    				tazeleme++; //Tazeleme sayısı arttırılıt
    				pilavSayi++; //Pilav sayısı arttırılır
    				price+=priceTableReorganize*taz; //Masaya tazemeleme sayısı kadar organize edilme ücreti eklenir.
				price+=(2*rice*taz); //Masaya tazemeleme sayısı kadar pilav için pirinç fiyatı eklenir
    			}
    		  	if(count==0){ //Masa dolduğunda masa ve masadaki filozoflara ait bilgiler yazdırılır
    		  		printf("\n ---- Table[%d] ----\n",masaNum); //Table bilgileri
    		  		printf("table [%d] : tazeleme sayısı  %d\n",masaNum,tazeleme); //Table daki tazeleme sayısı
    		  		printf("table [%d] : Pilav Sayisi : %d\n",masaNum ,pilavSayi); //Table daki kaç kez pilav yapıldığı sayısı
     				printf("table [%d] : Yenilen Toplam Pirinc : %.2f\n",masaNum ,2000-totalpirinc); //Yenilen pirinç miktarı sayısı
    				printf("table [%d] : Price  : %.2f\n\n",masaNum, price); //Masadaki toplam ücret 
    				masaNum++; //Masa sayısı arttırılır
    				count=8; //Masa boşaltılacağından masadaki sandalye count değeri 8 olur.
    				totalpirinc=2000;//pirinc bir sonraki masa için yenilenir   		  	
    		  	}

    		}
    	
  	 //free ile struct yapısındaki elemanların malloc ile ayırdığım yer boşaltılır
	free(table);
	free(filozoflar);
	//pthread_detach(filozoflar->thread); //Threadleri yok ettim.
	
	}else{
		printf("10 dan fazla"); //10 dan fazla ise işlem gerçekleşmez
		sem_init(&kilit,0,10);  //10 dan fazla ise semafor nesnesini ayarladım.
		sem_wait(&kilit); //Kitlenmeyi sağladım
		sleep(4); //4 saniye bekleterek
		sem_post(&kilit); //Kitlenmeyi serbest bıraktım
	}	
	
	sem_destroy(&lock); //lock mekanizmasını yok ettim.
	sem_destroy(&kilit); //kilit mekanizmasını yok

	return 0;	
}





void* yeme_dusunme(void *argument){	
	//degisken tanımları
	int again = 1;	
	double pirinc=100;
	
	struct Philosopher* filozof = (struct Philosopher*)argument; //struct yapısını tanımladım.
	
	while(again==1){ //Again 1 ise 
	
		srand(time(NULL));
		printf("Philosopher %d is Thinking\n", filozof->id); //Düşünen filozofu yazdırdım.
		int time = rand() % 5 + 1; //1 ve 5 arasında random deger oluşturdum
		sleep(time);//bu süre kadar bekleme gerçekleştirdim
				
		if (sem_trywait(&table[filozof->solCatal].mutex)==0 && sem_trywait(&table[filozof->sagCatal].mutex)==0){
		//table yapısı ile filozofun solCatalı ve sağCatalı almasını sagladım, eger basarılı olursa 0 doner sem_trywait kullanılarak hemen donmesi sağlanılır
			while(time>0){	//süre bitmediyse döngüye devam ettim
					if(totalpirinc>100){ //Eğer pirinç miktarı 100 üzerindeyse
						printf("Philosopher %d is Eating\n", filozof->id); //Yiyen fizolof id bilgisini yazar
					}

					if (!filozof->eatTimes){  //filozof yemediyse
						sem_wait(&lock); //Lock degeri sıfır olmayan bir sayıya sahip oluncaya kadar askıya alınır
						yemeyen--; //yemeyen fizolof sayısı azaltılır
						sem_post(&lock); //Threadlere izin vererek sıradaki işlemin çalışması için sinyali gönderir
					}

					filozof->eatTimes++; //filozofun yeme sayısı arttırılır
			//		sleep(rand() % 5 + 1);
					
					sem_post(&table[filozof->sagCatal].mutex); //Yukarıda wait yaptığım degerlerin threadlerine sinyal gondererek işlemi devam ettirdim.

					time =- time; //Süreyi 1 azaltarak süre bitmediği sürece yemeye devam ettirdim
				} 
				sem_post(&table[filozof->solCatal].mutex);//Yukarıda wait yaptığım degerlerin threadlerine sinyal gondererek işlemi devam ettirdim.
		}else {
			printf("Philosopher %d not eat \n", filozof->id);//yemeyen filozof bilgisini yazdırdım 

		}
		again = !yeme_kontrol(); //yeme kontrolden dönen degere göre işlem while döngüsüne geri döner ya da sonlanır.

		
	}
	
}

int yeme_kontrol(){ //Filozofların yeme durumu kontrol edilir.
	int cont; //Return değerini belirlemek için bir değişken tanımladım.
	sem_wait(&lock); //Kitlenmeyi gerçekleştirdim
	cont = yemeyen; //yemeyen sayısını cont değişkenine atadım
	sem_post(&lock); //Kitlenmeyi serbest bıraktım
	taz++; //Tazelenme sayısını arttırdım.
	return cont==0;

}
