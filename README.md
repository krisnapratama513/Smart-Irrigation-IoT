# Pengembangan Sistem Irigasi Cerdas Adaptif Berbasis IoT

Proyek ini adalah sistem irigasi otomatis menggunakan mikrokontroler ESP32 yang mengintegrasikan data sensor kelembaban tanah dengan data prediksi cuaca real-time dari API eksternal.

## 📁 Struktur Folder
Berdasarkan struktur repositori ini:
- `Smart_Irrigation.ino`: Kode utama untuk mikrokontroler ESP32.
- `README.md`: Dokumentasi utama proyek.
- `docs/Laporan_Final_Project.pdf`: Dokumen laporan akhir lengkap.
- `docs/Hasil Pengujian Logika Sensor.pdf`: Dokumentasi detail hasil pengujian sistem.

## 🛠️ Komponen Hardware
- **ESP32 WiFi Module**: Mikrokontroler utama dengan konektivitas WiFi.
- **Soil Moisture Sensor**: Mengukur tingkat kelembaban tanah secara real-time.
- **Sensor DHT11**: Mengukur suhu dan kelembaban udara sekitar.
- **Relay Module 1 Channel**: Saklar elektronik untuk mengontrol pompa air.
- **Water Pump Mini**: Aktuator untuk mendistribusikan air ke tanaman.

## 🧠 Metode Pengolahan Data (Rule Base)
Sistem menggunakan logika pengambilan keputusan hibrida (Hybrid Decision Making):

1. **Normalisasi Data**: Data sensor tanah dikonversi ke persentase (0-100%). Nilai <40% dianggap "Kering".
2. **Integrasi API**: Mengambil data cuaca dari OpenWeatherMap. Jika Weather ID < 600, status diatur menjadi "Akan Hujan".
3. **Logika Kontrol**:
   - **IF** Tanah Kering **AND** Tidak Akan Hujan **THEN** Pompa ON (Menyiram).
   - **IF** Tanah Kering **AND** Akan Hujan **THEN** Pompa OFF (Menunda/Hemat Air).
   - **IF** Tanah Basah **THEN** Pompa OFF (Standby).

## 📊 Hasil Pengujian
- **Skenario Tanah Kering**: Sistem berhasil mendeteksi kelembaban 10% dan menyalakan pompa.
- **Skenario Tanah Basah**: Sistem mendeteksi kelembaban 95% dan mematikan pompa secara otomatis.
- **Skenario Adaptif**: Sistem berhasil menunda penyiraman meskipun tanah kering karena mendeteksi status "Akan Hujan" dari API.

## 👨‍💻 Kelompok 11 - Informatika 4
- **Khoirudin Krisna Pratama** (22.11.4756)
- **Oktavian Endarwan** (22.11.4892)
- **Dosen Pengampu**: Ahmad Ridwan, S.Tr.T., M.T