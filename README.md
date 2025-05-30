# Final-Exam-IOT-BINUS
kode ESP32 untuk final exam IOT 


- Terdapat 3 rack server dan 3 Microcontroller ESP32 pada masing-masing rack server. Masing-masing ESP32 memiliki sensor suhu dan kelembaban DHT22
- Semua program hampir sama persis karena tugasnya sama dan tujuannya juga sama,hanya beda di masing-masing token device di Thingsboard nya saja.
- Protokol komunikasi antara microcontroller dengan cloud thingsboard yang digunakan adalah MQTT, yang dimana microcontroller sebagai publisher dan Thingsboard sebagai subscriber.
- Untuk microcontroller di rack server 1 dan 3 memanfaatkan simulator Wokwi sedangkan microcontroller di rack server 2 menggunakan ESP32 dan sensor DHT22 asli yang diprogram dari Ardunio IDE.
