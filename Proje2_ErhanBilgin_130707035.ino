void(* reset) (void) = 0; //arduino reset fonksiyonu
void alti_led();//switch değerine göre ledleri yakar
void gecici_buton();//butona basılma durumunda girer
void degerler();//2 bitlik değerleri 10 tabanına çevirir
void yazdir(int);//serial port ekranına rastgele ve kullanıcı tahmini yazdırır
void tahmin();//switch değerine ile rastgele sayı işlemleri yapar
int switchNo[6],rastgeleSayi[6],kalanhak[3];//swithNo 6 bitlik dizi
//rastgeleSayi 6 bitlik dizi kalanhak 3 bitlik dizi
int sayac=0,r_ondalik=0,s_ondalik=0,sayi=1;
//sayac kalan hak için kullanılır
//r_ondalik-> rastgele sayısının 10 tabanındaki karşılığı
//s_ondalik-> Switch deki sayının 10 tabanındaki karşılığı
int esitmi=0;
//sayıların birbirine eşit olup olmaması için kullanılan değişken
void setup() {
  // put your setup code here, to run once:
  for (int i=2;i<8;i++)
  {//2 ile 7 arasındaki digital pinler giriş
    //8 ile 13 arasındaki digital pinler çıkış olarak tanımlanır
    pinMode(i,INPUT);
    pinMode(i+6,OUTPUT);
  }
  //A1 buton için giriş olarak belirlenir
  pinMode(A1,INPUT);
  //A2 ve A3 ise mavi ve yeşil led için çıkış belirlenir
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  //rand fonksiyonu için A0 dan çekirdek değeri alınır
  randomSeed(analogRead(A0));
  //Serial port ekranı tanımlanır
  Serial.begin(9600);
  //PORT B ile 8-13 arasındaki ledler kontrol edilerek
  //söndürülür
  PORTB = B000000;
}

void loop() {
  altiled();//alti led fonksiyonuna gider
  degerler();//degerler fonksiyonuna gider
  yazdir(1);//Serial port ekranında yazdırma fonksiyonu
  if (analogRead(A1)>=1022)
  {//A1 değeri 1022 eşit ve büyükse
    //gecici_buton fonksiyonuna ardından
    //tahmin fonksiyonuna gider
    gecici_buton();
    tahmin();
  }
}
void altiled()
{
  if (sayac<7 && sayac>0)
  {//eğer sayacımız 0 ile 7 arasında ise 
    //yani butona ilk kez basıyorsak girmez
    int j=5;
    //bu kısımda switcde okunan değeri
    //switchNo dizisine atar
    //kabloları bağlama şeklimden dolayı 
    //switch açıkken 0 değeri okumakta 
    //o yüzden diziye 1 değeri atıyor.
    for (int i=2;i<8;i++)
    {
      if (digitalRead(i)==0)
      {
        //diziye atadığı değere göre 
        //ledlerimizi yakıyor veya 
        //söndürüyor
        switchNo[j]=1;
        digitalWrite(i+6,HIGH);
      }
      //eğer switch in değeri 1 ise de
      //0 değerini diziye atıyor
      else
      {
        switchNo[j]=0;
        digitalWrite(i+6,LOW);
      }
      //switchNo nun en son indisinden
      //atama yaptığı için j yi bir eksiltiyor
      j--;
    }
  }
}
void gecici_buton()
{
  //gecici butonda butona 2. kez basıldığında bu ife girer
  if (sayac!=0 && sayac<7)
  {//ve üretilen rastgele sayı eşitmi diye bakar
    for (int i=0;i<6;i++)
    {
      if (rastgeleSayi[i]==switchNo[i])
      {
        esitmi=1;
      }
      else
      {
        esitmi=0;
        break;
      }
    }
    //ve kalan hak sayısını tutan sayac azaltılır
    sayac++;
  }
  if (sayac==0)
  {//eğer sayac 0 a eşit ise butona ilk kez basılmıştır
    for (int i=0;i<6;i++)
    {//bu kısımda rastgeleSayi dizisine random fonksiyonu ile
      //0 ile 1 arasında rastgele sayılar eklenir
      rastgeleSayi[i]=random(0,2);
    }
    //PORTB ile 8-13 arasındaki digital pinlere bağlı
    //ledler 1 saniye süresince yanar ve söner.
    PORTB = B111111;
    delay(1000);
    PORTB = B000000;
    //ve son olarak birdaha buraya girmemek için sayac
    //değişkeni 1 e eşitlenir
    sayac=1;
  }
}
void degerler()
{//r_ondalik ve s_ondalik değişkenlerine
  //2 bitlik sayıların 10 tabanındaki değeri atılır
  r_ondalik=0;
  s_ondalik=0;
  for (int i=0;i<6;i++)
  {//pow fonksiyonu pow(2,5) olduğunda 63 değeri verdiğinden
    //dolayı 2 üzeri kısmını for döngüsünde hesapladım
    for (int j=0;j<i;j++)
    {
      sayi*=2;
    }
    //daha sonra dizideki değerine göre çarpıp topladım
    r_ondalik+=(rastgeleSayi[5-i]*sayi);
    s_ondalik+=(switchNo[5-i]*sayi);
    //tabi sayı değeri her baştan başlamasında 1 eşit olması gerekiyor.
    sayi=1;
  }
}
void yazdir(int yazdir1)
{//bu fonksiyon switch ve rastgele sayısının ikilik ve ondalık değerlerini
  //serial port ekranında ekrana yazdırmaktadır
  //tabi loop fonksiyonun da yazdir fonksiyonunu 1 parametresi gönderirsek
  //eğer sıfır döndürürsek bu fonksiyonun işlemlerini yapmayacaktır.
  if (yazdir1==1)
  {
    Serial.println();
    Serial.println("Switchin 2 lik ve ondalik değerleri");
    for (int i=0;i<6;i++)
    {
      Serial.print(switchNo[i]);
    }
    //serial port ekranında güzel gözükmesi için
    //gereken println fonksiyonları vardır.
    Serial.println();
    Serial.println("Switchin ondalik değeri");
    Serial.println(s_ondalik);
    Serial.println();
    Serial.println("Rastgele sayinin 2 lik ve ondalik değerleri");
    for (int i=0;i<6;i++)
    {
      Serial.print(rastgeleSayi[i]);
    }
    Serial.println();
    Serial.println("Rastgele sayi ondalik değeri");
    Serial.println(r_ondalik);
  }
}
void tahmin()
{
  //tahmin fonksiyonu girilen switch değerinin
  //10 tabanındaki değeri ni rastgele sayının 10 
  //tabanındaki değeri ile karşılaştırmaktadır
  if (esitmi==0 && sayac>1)
  {//eğer girilen sayı eşit değilse bu ife girer
    if (s_ondalik>r_ondalik)
    {//ve girilen sayı rastgele sayıdan büyükse
      for (int i=sayac;i<7;i++)
      {//mavi led kalan tahmin hakkı kadar 500 milisaniye
        //yanıp söner
        digitalWrite(A2,HIGH);
        delay(500);
        digitalWrite(A2,LOW);
        delay(500);
      }
    }
    else
    {//eğer küçükse girilen switch değeri
     for (int i=sayac;i<7;i++)
     {//yeşil led kalan tahmin hakkı kadar
      //500 milisaniye yanıp söner
      digitalWrite(A3,HIGH);
      delay(500);
      digitalWrite(A3,LOW);
      delay(500);
     }
    }
  }
  if (esitmi==1)
  {//eğer tahmin edilen sayı doğru ise
    //PORTB ile 8-13 arasındaki pinler söndürülür
    PORTB = B000000;
    //ve kalan tahmin hakkı hesaplanıp 2 bitlik değere çevrilir
    sayac=8-sayac;
    for (int i=0;i<3;i++)
    {
      //ve diziye atılır
      if (sayac%2==1)
      {
        kalanhak[3-i]=1;
      }
      else
      {
        kalanhak[3-i]=0;
      }
      sayac/=2;
    }
    int j=3;
    //diziye atma işlemi bittikten sonra 
    //kalanhak dizisindeki sayının bit değeri 
    //ilgili ledleri yakar
    for (int i=8;i<11;i++)
    {
      if (kalanhak[j]==1)
      {
        digitalWrite(i,HIGH);
      }
      else
      {
        digitalWrite(i,LOW);
      }
      j--;
    }
    //5 saniye boyunca kalan tahmin hakkının
    //2 bitlik değeri ilgili ledleri yakar
    delay(5000);
    //daha sonra reset fonksiyonu çağırılarak
    //arduino yeniden başlar
    reset();
  }
  if (sayac==7)
  {//eğer kullanıcı tüm haklarını kullanırsa
    PORTB = B000000;
    //8-13 arasındaki ledlerin tamamı söndürülür.
    for (int i=8;i<14;i++)
    {//rastgele sayının 2 bitlik
      //değeri ilgili ledleri yakar veya söndürür.
      if (rastgeleSayi[13-i]==0)
      {
        digitalWrite(i,LOW);
      }
      else
      {
        digitalWrite(i,HIGH);
      }
    }
    //5 saniye bu halde bekler ve 
    //arduino reset fonksiyonu sayesinde yeniden başlatılır.
    delay(5000);
    reset();
  }
}

