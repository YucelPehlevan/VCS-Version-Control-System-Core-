# mini-VCS
# 🇬🇧 English

📌 About the Project
mini_VCS is a minimal version control system core written in C++, designed to explore and understand the fundamental ideas behind systems like Git — without the complexity.
The project focuses on:
Learning how version control works internally
Practicing file system operations in C++
Building a clean, reusable core logic independent from UI (CLI / GUI)
Each version stores a snapshot of the file along with metadata.

⚙️ Features
Save file versions (save)
List existing versions (list) 
Restore a previous version (go_to_version)
Delete versions (delete_version)
Check file status (show_status)
Version metadata (timestamp & message)
Automatic version directory handling

🧠 Design Notes
Versions are stored in versions_of_<filename>
Each version consists of:
vN → file snapshot
vN_info → version metadata
Uses std::filesystem for portable file handling
Intentionally kept simple and educational

🛠️ Build
Requires a C++17 compatible compiler.
g++ -std=c++17 main.cpp VCS.cpp -o mini_vcs

# 🇹🇷 Türkçe

📌 Proje Hakkında
mini_VCS, tek bir dosya üzerinde çalışan, temel sürüm kontrol (version control) mantığını öğretmek ve denemek amacıyla geliştirilmiş C++ tabanlı minimal bir VCS çekirdeğidir.
Bu proje:
Git gibi büyük sistemlerin çekirdek mantığını anlamaya,
Dosya versiyonlama, geri alma ve karşılaştırma kavramlarını öğrenmeye,
GUI veya CLI’den bağımsız, core (çekirdek) bir yapı kurmaya odaklanır.
Her sürüm, ilgili dosyanın bir kopyası ve metadata bilgisiyle birlikte saklanır.

⚙️ Özellikler
Dosya sürümü kaydetme (save)
Kayıtlı sürümleri listeleme (list)
Önceki sürüme geri dönme (go_to_version)
Sürüm silme (delete_version)
Dosya durumu kontrolü (show_status)
Zaman damgası ve kullanıcı mesajı ile sürüm bilgisi
Otomatik versiyon klasörü yönetimi

🧠 Tasarım Notları
Sürümler versions_of_<dosya_adı> klasörü altında tutulur
Her sürüm:
vN → dosyanın kendisi
vN_info → tarih, mesaj ve sürüm bilgisi
std::filesystem kullanılarak platform uyumlu dosya işlemleri yapılır
Proje bilinçli olarak minimal tutulmuştur (eğitim ve öğrenme amaçlı)

🛠️ Derleme
C++17 uyumlu bir derleyici yeterlidir.
g++ -std=c++17 main.cpp VCS.cpp -o mini_vcs


