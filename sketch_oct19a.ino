  #include <LiquidCrystal_I2C.h>
#include <Keypad.h>

//define state of fsm
#define init_mode 0
#define select 1

#define  MODE1 2
#define  RUN_MODE1 3

#define  MODE2 10
#define  RUN_MODE2 11
#define  PAUSE_MODE2 12


#define END 99

#define TIMER 500 //gia lap thoi gian di het 1km

//Khởi tạo với các chân
LiquidCrystal_I2C lcd(0x3F,16,2);//SETUP LCD I2C//3F
const byte ROW_NUM = 4;  
const byte COLUMN_NUM = 4; 

// Ta thay button '*' thành 'C','#' thành 'R','C' thành '/'
char keys[ROW_NUM][COLUMN_NUM] = {//MAPPING KEYPAD
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', '/' },
  { 'C', '0', 'R', 'D' }
};
//S stand for ans


byte pin_rows[ROW_NUM] = { 9, 8, 7, 6 };       //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = { 5, 4, 3, 2 };  //connect to the column pinouts of the keypad



Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM); 

long int t=0;//DISTANCE
int state=init_mode;//STATUS OF FSM
long int money_spend=0;//MONEY SPEND
int length=0;
int flag=0;
//ham kiem tra so chu so
int check(int t){
    if(t<10) return 1;
    return check(t/10)+1;
}

void setup() {
  lcd.init();
  lcd.backlight();
//INIT AND TURN ON LCD
}

void loop() {
  
  switch(state)
  {
    case  init_mode://INIT
    {
      lcd.begin(16, 2);
      lcd.print("Chon che do:");
      lcd.setCursor(0, 1);
      lcd.print("A:MODE1 B:MODE2");
      t=0;
      state=select;
      length = 0;
      flag=0;
      money_spend=0; 
    }
    case  select://select MODE
    {
      t=0;
      flag=0;
      length = 0;
      money_spend=0;
      char key = keypad.getKey();
      if(key == 'A'){// MODE 1
        lcd.clear();
        lcd.begin(16, 2);
        lcd.print("Nhap so tien:");
        state=MODE1;
      }
      if(key == 'B'){// MODE 2
        lcd.clear();
        state=MODE2;
      }
    break;
    }
    case  MODE1://MODE 1
    {
    char customKey = keypad.getKey();
    if(customKey){

        if(customKey!='D'&&customKey!='R'&&customKey!='C'&&customKey!='/'&&customKey!='A'&&customKey!='B'){//INPUT MONEY
          t=t*10;
          t=t+customKey;
          t-=48;
          lcd.setCursor(0, 1);
          lcd.println(t);      
        }
        else if (customKey=='D'){// An nut "D" de hoan tat qua trinh nhap tien va chuyen sang trang thai chay
          state=RUN_MODE1;
        }
        else  if (customKey=='R'){//An nut "R" de reset
          lcd.clear();
          state=init_mode;
        }
        else if(customKey=='C'){//An nut "C" de xoa so tien dang nhap va nhap lai
          t=0;
          lcd.begin(16, 2);
          lcd.print("Nhap so tien:");
        }
    }
      break;
    }
    case  3:// RUN MODE 1
    {
      char customKey = keypad.getKey();
      if(customKey){
        if (customKey=='D'){// Nhan D de hien thi so tien va cam on khach hang
          lcd.clear();
          state=END;
        }
        else if (customKey=='R'){// Nhan R de reset trang thai ve select mode
          lcd.clear();
          state=init_mode;
        }
      }
      if(t>=0)//in ra man hinh thong tin cua chuyen di
      {
        int loc=0;
        lcd.setCursor(0, 0); 
        lcd.print("So km // so tien");
      
        lcd.setCursor(loc, 1);
        lcd.print(length);
        loc+=check(length); 
        length+=1;
      
        lcd.setCursor(loc, 1); 
        lcd.print("//");

        loc+=2;
        lcd.setCursor(loc, 1); 
        lcd.print(money_spend);
        money_spend+=5;
      
        t-=5;
        delay(TIMER);//delay 0.5s(mo phong chuyen dong cua xe di 1km/0.5s)
        
      }
      break;
    }
    case  10://MODE2
    {
      int loc=0;
      lcd.setCursor(0, 0); 
      lcd.print("Bam D de chay");
      char customKey = keypad.getKey();
      if(customKey){
        if (customKey=='R'){// bam R de reset fsm ve lai select MODE
          lcd.clear();
          state=init_mode;
        }
        if(customKey=='D'){// bam D de xac nhan chay va chuyen sang RUN MODE 2
          state=RUN_MODE2;
        }

      }
        
      break;
    }
    case  11://RUN MODE 2;
    {

      char customKey = keypad.getKey();
      if(customKey){
        if(customKey=='D'){
          lcd.clear();
          state=PAUSE_MODE2;
          
        }


      }else{
        int loc=0;
        lcd.setCursor(0, 0); 
        lcd.print("So km // so tien");
      
        lcd.setCursor(loc, 1);
        lcd.print(length);
        loc+=check(length); 
        length+=1;
      
        lcd.setCursor(loc, 1); 
        lcd.print("//");

        loc+=2;
        lcd.setCursor(loc, 1); 
        lcd.print(money_spend);
        money_spend+=5;
      
        t-=5;
        delay(TIMER);//delay 0.5s    
      }
      break;
    }
    case  12://Pause MODE2
    {
      if(flag==0){
        int loc =0;
        lcd.setCursor(0, 0); 
        lcd.print("Tong km // tien");
      
        lcd.setCursor(loc, 1);
        lcd.print(length);
        loc+=check(length); 
        length+=1;
      
        lcd.setCursor(loc, 1); 
        lcd.print("//");

        loc+=2;
        lcd.setCursor(loc, 1); 
        lcd.print(money_spend);
        flag=1;
      }
      char customKey = keypad.getKey();
      if(customKey){
        if(customKey=='D'){
          lcd.clear();
          state=END;
        }


      }      
      break;
    }
    case  END://End
    {
        lcd.setCursor(1, 0); 
        lcd.print("Xin cam on quy khach");
        lcd.setCursor(4, 1); 
        lcd.print("quy khach");
        delay(3000);
        state=init_mode;
        money_spend=0;
        t=0;
        flag=0;
        length = 0;
    }
  }


  }
  
