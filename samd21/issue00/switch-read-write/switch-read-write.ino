/*
 * Connections: 
 *  Wire D2 to A0
 *  Wire D3 to A1
 */

bool ai, bi, ao, bo;
double attempts, errors;
int ci;
  
void setup() {
  // put your setup code here, to run once:
  pinMode(14,INPUT); //A0
  pinMode(15,INPUT); //A1
  pinMode(2,OUTPUT);//D2
  pinMode(3,OUTPUT); //D3
  Serial.begin(57600);
  bo = !ao;
}

void loop() {
  // put your main code here, to run repeatedly:
  attempts++;
  ao = !ao;
  bo = !bo;
  digitalWrite(2, ao); 
  digitalWrite(3, bo);
  delay(1);
  ai = digitalRead(14);
  bi = digitalRead(15);
  ci = analogRead(15);
  if (ao != ai) {
    errors++;   
  }
   if (bo != bi) {
    errors++;   
  }
  Serial.print("Errors ");
  Serial.print(errors);
  Serial.print("/");
  Serial.print(attempts);
  Serial.print(" attempts [");
  Serial.print(ao);
  Serial.print(bo);
  Serial.print("->");
  Serial.print(ai);
  Serial.print(bi);
  Serial.print("] (ignore: ");
  Serial.print(ci); //print var value to avoid compiler optimising away the read 
  Serial.print(")\n");
  
  
}
