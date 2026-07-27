#include <Servo.h>

// ============================================================
// مشروع بوابة العبور (الضبط النهائي للزوايا)
// ============================================================

#define RED_LED_PIN    2   // اللمبة الحمراء (البوابة مغلقة)
#define YELLOW_LED_PIN 4   // اللمبة الصفراء (البوابة مفتوحة)
#define SERVO_PIN      9   // إشارة السيرفو
#define TRIG_PIN       10  // إرسال الألتراسونيك
#define ECHO_PIN       11  // استقبال الألتراسونيك

Servo gateServo;

const int DISTANCE_THRESHOLD = 10; // مسافة التفعيل (10 سم)

// التوزيع الهندسي الصحيح للزوايا
const int GATE_CLOSED_ANGLE = 0;  // الوضع الطبيعي (البوابة مغلقة أفقياً)
const int GATE_OPEN_ANGLE   = 180; // عند التقريب (ترتفع البوابة لأعلى)

// ------------------------------------------------------------
// دالة قياس المسافة
// ------------------------------------------------------------
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

// ------------------------------------------------------------
// الإعداد الأولي (Setup)
// ------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);

  gateServo.attach(SERVO_PIN);
  
  // ضبط البوابة على زاوية المنتصف (90) وتضيء الحمراء
  gateServo.write(GATE_CLOSED_ANGLE);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(YELLOW_LED_PIN, LOW);

  delay(500);
}

// ------------------------------------------------------------
// الدورة الرئيسية (Loop)
// ------------------------------------------------------------
void loop() {
  long distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // الشرط: عند اقتراب جسم لمسافة 10 سم أو أقل
  if (distance <= DISTANCE_THRESHOLD && distance > 0) {
    gateServo.write(GATE_OPEN_ANGLE);   // ترتفع البوابة لأعلى (180 درجة)
    digitalWrite(RED_LED_PIN, LOW);     // تنطفئ الحمراء
    digitalWrite(YELLOW_LED_PIN, HIGH); // تضيء الصفراء
  } 
  else {
    gateServo.write(GATE_CLOSED_ANGLE); // تعود البوابة للوضع الأفقي (90 درجة)
    digitalWrite(RED_LED_PIN, HIGH);    // تضيء الحمراء
    digitalWrite(YELLOW_LED_PIN, LOW);  // تنطفئ الصفراء
  }

  delay(100);
}