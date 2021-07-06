Witajcie w mojej kuchni

W tej bibliotece postaram si� zawrze� funkcje dotycz�ce silnik�w Pololu HPCB 50:1 wraz z enkoderami do robota SCALAK
Czyjniki chyba te� tu wrzuc�.

by Eryk Mo�d�e� stycze� 2019r.

**EDIT**
Jest tu wszystko do tego robota z wyjątkiem enkoderów.

Może kiedyś dodam coś takiego jak "klasa robot". Będą tam funkcje typu turn(power,radius,side) albo returnToCenter().
Zostało 2 dni do zawodów, teraz nie ma na to czasu xD.

**EDIT 19.05.2019**
Plan sie trochę pozmieniał, lecz wszystko śmiga. Jest pare innych klas do nowych elementów Scalaka.
'klasa robot' to syf nie polecam. Kusi mnie dodać kod do ledów i serv co napewno kiedyś zrobię.

**EDIT 15.08.2019**
Podzieliłem pliki na definicje poszczególnych klas. Moduł startowy dostaje funkcje, którą ma wykonywać w przypadku zatrzymania, co wydaję się logiczniejsze.

**EDIT 2.10.2019**
Miałem enkodery i działały perfekcyjnie, lecz były dość awaryjne więc je wymontowałem. Moduł bluetooth fajna rzecz, lecz dziadoski SoftwareSerial nie ogarnia życia, a osobnych wyprowadzeń UART nie zamierzam robić. Kable silników zmienione na grubsze a ich wtyki na niższe. Dodałem klase Led (ten wbudowany w pin 13) i ładnie śmiga. Wszystkie ustawienia początkowe obiektów wsadziłem do inicjalizacji. A i jeszcze guzik do startowania i stopowania się pojawił.