#include <Arduino.h>
#include "stack.c"
#include "dictionary.c"
#include "rpn.c"
#include "script_reader.c"
#include "script_decoder.c"


static char* script_text = "\
10 I%=0\n\
20 I%=I%+1\n\
30 IF I%<10 THEN 20 ELSE 40\n\
40 S$=\"OK\"\n\
50 END\n\
60 S$=\"##HELLO##\"\n\
";


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  delay(100);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("Hello world");

  reader_init(script_text);
  dic_clear();

#if 0
  Serial.println("----");

  while (0 != reader_next()) {
    Serial.print("TOKEN:");
    Serial.print((int)reader.token);
    Serial.print(" ");
    Serial.print("CONTEXT:");
    Serial.print(reader.context);
    Serial.println("");
  }
#else
  while (0 != decoder_execute()) {
    Serial.println("---");
  }
  Serial.println(dic_get("I%"));
  Serial.println(dic_get("S$"));
  Serial.println("============");
#endif

  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);

}




/*
  dic_set("data1", "hello");
  dic_set("data2", "world");
  dic_set("data2", "foo");

  Serial.println(dic_get("data1"));
  Serial.println(dic_get("data2"));

  stack_push(123);
  stack_push(456);

  int a1 = stack_pop();
  int a2 = stack_pop();
  Serial.println(a1);
  Serial.println(a2);

  //文字を数字に変換
  Serial.println(strtol("5512", NULL, 0));
  Serial.println(strtol("0x20", NULL, 0));

  //数字を文字に変換
  char dat[16];
  Serial.println(itoa(4321, dat, 10));
*/
