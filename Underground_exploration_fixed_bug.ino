#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define leftButton 11
#define rightButton 10
#define middleButton 9
uint8_t table[8] = {0x00, 0x0a, 0x00, 0x15, 0x15, 0x0a, 0x00, 0x00};
uint8_t person[8] = {0x00, 0x04, 0x0a, 0x04, 0x15, 0x0a, 0x11, 0x00};
uint8_t soil[8] = {0x15, 0x0a, 0x15, 0x0a, 0x15, 0x0a, 0x15, 0x0a};
uint8_t coin[8] = {0x00, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x0e, 0x00};
int xPerson = 0;
int yPerson = 0;
int coinFigure = 0;
int floorFigure = 0;
int bomb = 0;
int second[] = {1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1};

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  // put your setup code here, to run once:
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  pinMode(11,INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Press any key to");
  lcd.setCursor(0,1);
  lcd.print("start");
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  pause();
  lcd.clear();
  lcd.print("By alpha station");
  delay(2000);


  lcd.createChar(0, table);
  lcd.createChar(1, person);
  lcd.createChar(2, soil);
  lcd.createChar(3, coin);
  {  
    lcd.clear();
    lcd.print(char(0));
    lcd.print(" Hello,This is");
    lcd.setCursor(0,1);
    lcd.print("my shop");
    pause();
    lcd.clear();
    lcd.print("you: 0$");
    delay(2000);
    pause();
    randomSeed(analogRead(A0));
    int first[] = {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0};
    //randomSecond();
    while(true)
    {
      lcd.clear();
      if(second[xPerson] == 3)//吃脚下的金币
      {
        second[xPerson] = 0;
        coinFigure += 1;
      }
      if(second[xPerson] == 0)//走到下一层
      {
        floorFigure += 1;
        for(int i = 0; i < 16; i++)
        {
          first[i] = second[i];
        }
        randomSeed(analogRead(A0));
        randomSecond();
        delay(500);
      }
      if(!digitalRead(rightButton))//向右移动
      {
        if(xPerson == 0)
        {
          if(first[15] == 0)
          {
            xPerson--;
          }
          else if(first[15] == 3)//吃右边金币
          {
            xPerson--;
            coinFigure += 1;
            first[15] = 0;
          }
        }
        else
        {
          if(first[xPerson - 1] == 0)
          {
            xPerson--;
          }
          else if(first[xPerson - 1] == 3)//吃右边金币
          {
            xPerson--;
            coinFigure += 1;
            first[xPerson] = 0;
          }
        } 
      }
      if(!digitalRead(leftButton))//向左移动
      {
        if(xPerson == 15)
        {
          if(first[0] == 0)
          {
            xPerson++;
          }
          else if(first[0] == 3)//吃左边金币
          {
            xPerson++;
            coinFigure += 1;
            first[0] = 0;
          }
        }
        else
        {
          if(first[xPerson + 1] == 0)
          {
            xPerson++;
          }
          else if(first[xPerson + 1] == 3)//吃左边金币
          {
            xPerson++;
            coinFigure += 1;
            first[xPerson] = 0;
          }
        } 
      }
      if(!digitalRead(middleButton))//挖土
      {
        if(second[xPerson] == 2)
        {
          second[xPerson] = 0;
        }
        else if(second[xPerson] == 1)
        {
          if(bomb)//炸石头
          {
            bomb--;
            second[xPerson] = 0;
          }
        }
      }

      if(xPerson > 15)//离开右边界
      {
        xPerson = 0;
      }
      if(xPerson < 0)//离开左边界
      {
        xPerson = 15;
      }

      for(int i = 0; i < 16; i++)//打印第一层地形
      {
        lcd.setCursor(i,0);
        if(first[i] == 1)
        {
          lcd.print(char(0xff));
        }
        else if(first[i] == 2)
        {
          lcd.print(char(2));
        }
        else if(first[i] == 3)
        {
          lcd.print(char(3));
        }
      }
      for(int i = 0; i < 16; i++)//打印第二层地形
      {
        lcd.setCursor(i,1);
        if(second[i] == 1)
        {
          lcd.print(char(0xff));
        }
        else if(second[i] == 2)
        {
          lcd.print(char(2));
        }
        else if(second[i] == 3)
        {
          lcd.print(char(3));
        }
      }
      lcd.setCursor(xPerson, yPerson);
      lcd.print(char(1));//打印出人
      if(bomb == 0)
      {
        if(xPerson == 0)//判断是否失败
        {
          if(first[15] == 1 || first[15] == 2)
          {
           if(first[1] == 1 || first[1] == 2)
            {
             if(second[1] == 1)
             {
              delay(2000);
              lcd.setCursor(2,1);
              lcd.print("You lost"); 
              delay(2000);
              break;
             }
            }
          }
        }
      else if(xPerson == 15)//判断是否失败
      {
        if(first[0] == 1 || first[0] == 2)
        {
          if(first[14] == 1 || first[14] == 2)
          {
            if(second[15] == 1)
            {
              delay(2000);
              lcd.setCursor(2,1);
              lcd.print("You lost"); 
              delay(2000);
              break;
            }
          }
        }
      }
      else
      {
        if(first[xPerson + 1] == 1 || first[xPerson + 1] == 2)
        {
          if(first[xPerson - 1] == 1 || first[xPerson - 1] == 2)
          {
            if(second[xPerson] == 1)
            {
              delay(2000);
              lcd.setCursor(0,1);
              lcd.print("You lost"); 
              delay(2000);
              break;
            }
          }
        }
      }
      }
      delay(200);
    }
  }
  lcd.clear();
  lcd.print("floor:");
  lcd.print(floorFigure);
  lcd.setCursor(0,1);
  lcd.print("coin:");
  lcd.print(coinFigure);
  delay(2000);
  pause();
  lcd.clear();
  lcd.print("Thanks for playi");
  lcd.setCursor(0,1);
  lcd.print("ng");
  floorFigure = 0;
  coinFigure = 0;
  xPerson = 0;
  for(int i = 0; i < 16; i++)
  {
    if(i < 6 || i > 11)
    {
      second[i] = 1;
    }
    else
    {
      second[i] = 0;
    }
  }
  delay(2000);
}

void pause(void)
{
    while(true)
    {
      if(!digitalRead(leftButton) || !digitalRead(rightButton) || !digitalRead(middleButton))
      {
        break;
      }
    }
}

void randomSecond(void)
{
  for(int i = 0; i < 16; i++)
  {
    if(random(10) < 2) 
    {
      second[i] = 0;
    }
    else if(random(10) < 6) 
    {
      second[i] = 2;
    }
    else if(random(10) < 7) 
    {
      second[i] = 3;
    }
    else
    {
      second[i] = 1;
    }
  }
  if(floorFigure % 5 == 0)//判断商人是否出现
  {
    if(random(10) < 3)
    {
      businessman();
    }
  }
}

void businessman(void)
{
  lcd.clear();
  lcd.print(char(0));
  lcd.print(":Its ");
  lcd.print(floorFigure);
  lcd.print("floor!");
  lcd.setCursor(0,1);
  lcd.print("I'm coming!!!");
  delay(2000);
  pause();
  lcd.clear();
  lcd.print("You: ");
  lcd.print(coinFigure);
  lcd.print("$");
  delay(2000);
  pause();
  if(coinFigure > 10)
  {
    lcd.clear();
    lcd.print(char(0));
    lcd.print(":Do you want a");
    lcd.setCursor(0,1);
    lcd.print("bomb? It can");
    delay(2000);
    pause();
    lcd.clear();
    lcd.print("bomb? It can");
    lcd.setCursor(0,1);
    lcd.print("destroy rock");
    delay(2000);
    pause();
    lcd.clear();
    lcd.print("A bomb 10$");
    lcd.setCursor(0,1);
    lcd.print("middle: Buy");
    delay(2000);
    while(true)
    {
      if(!digitalRead(leftButton) || !digitalRead(rightButton))
      {
        break;
      }
      else if(!digitalRead(middleButton))
      {
        coinFigure -= 10;
        bomb += 1;
        break;
      }
    }
  }
  else
  {
    lcd.clear();
    lcd.print(char(0));
    lcd.print(":...I'll give u");
    lcd.setCursor(0,1);
    lcd.print("something when");
    delay(2000);
    pause();
    lcd.clear();
    lcd.print("something when");
    lcd.setCursor(0,1);
    lcd.print("you have 10$");
    delay(2000);
    pause();
  }
}
