/******************************************************************
 Widzenie Komputerowe  - program  przyk³adowy
 *****************************************************************/

#include <windows.h>
#include <vector>

using namespace std;

struct DPOINT
{
	double x;
	double y;
};

int aktKlasa = 0;
DPOINT klasa0[100];
int licz0 = 0;
DPOINT klasa1[100];
int licz1 = 0;
DPOINT klasa2[100];
int licz2 = 0;
DPOINT klasa3[100];
int licz3 = 0;

//deklaracja funkcji obslugi okna
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL RysujUklad(HDC kontekst);
BOOL RysujPunkty(HDC kontekst);
BOOL RysujPunkt(HDC kontekst, POINT pt, COLORREF kolor);
DPOINT OknoNaUklad(POINT ptOkna);
POINT UkladNaOkno(DPOINT ptUkladu);
DPOINT WyszukajSrodek(DPOINT __klasa[], int __licz);
BOOL RysujPixel(HDC kontekst, POINT pt, COLORREF kolor);

//funkcja Main - dla Windows
 int WINAPI WinMain(HINSTANCE hInstance,
               HINSTANCE hPrevInstance,
               LPSTR     lpCmdLine,
               int       nCmdShow)
{
	MSG meldunek;		  //innymi slowy "komunikat"
	WNDCLASS nasza_klasa; //klasa g³ównego okna aplikacji
	HWND okno;
	static char nazwa_klasy[] = "Podstawowa";

	//Definiujemy klase g³ównego okna aplikacji
	//Okreslamy tu wlasciwosci okna, szczegoly wygladu oraz
	//adres funkcji przetwarzajacej komunikaty
	nasza_klasa.style         = CS_HREDRAW | CS_VREDRAW;
	nasza_klasa.lpfnWndProc   = WndProc; //adres funkcji realizuj¹cej przetwarzanie meldunków 
 	nasza_klasa.cbClsExtra    = 0 ;
	nasza_klasa.cbWndExtra    = 0 ;
	nasza_klasa.hInstance     = hInstance; //identyfikator procesu przekazany przez MS Windows podczas uruchamiania programu
	nasza_klasa.hIcon         = 0;
	nasza_klasa.hCursor       = LoadCursor(0, IDC_ARROW);
	nasza_klasa.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	nasza_klasa.lpszMenuName  = "Menu" ;
	nasza_klasa.lpszClassName = nazwa_klasy;

    //teraz rejestrujemy klasê okna g³ównego
    RegisterClass (&nasza_klasa);
	
	/*tworzymy okno g³ówne
	okno bêdzie mia³o zmienne rozmiary, listwê z tytu³em, menu systemowym
	i przyciskami do zwijania do ikony i rozwijania na ca³y ekran, po utworzeniu
	bêdzie widoczne na ekranie */
 	okno = CreateWindow(nazwa_klasy, "Widzenie komputerowe", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						CW_USEDEFAULT, CW_USEDEFAULT, 700, 600, NULL, NULL, hInstance, NULL);
	
	
	/* wybór rozmiaru i usytuowania okna pozostawiamy systemowi MS Windows */
   	ShowWindow (okno, nCmdShow) ;
    
	//odswiezamy zawartosc okna
	UpdateWindow (okno) ;

	// G£ÓWNA PÊTLA PROGRAMU
	while (GetMessage(&meldunek, NULL, 0, 0))
     /* pobranie komunikatu z kolejki; funkcja GetMessage zwraca FALSE tylko dla
	 komunikatu wm_Quit; dla wszystkich pozosta³ych komunikatów zwraca wartoœæ TRUE */
	{
		TranslateMessage(&meldunek); // wstêpna obróbka komunikatu
		DispatchMessage(&meldunek);  // przekazanie komunikatu w³aœciwemu adresatowi (czyli funkcji obslugujacej odpowiednie okno)
	}
	return (int)meldunek.wParam;
}

/********************************************************************
FUNKCJA OKNA realizujaca przetwarzanie meldunków kierowanych do okna aplikacji*/
LRESULT CALLBACK WndProc (HWND okno, UINT kod_meldunku, WPARAM wParam, LPARAM lParam)
{
	static HMENU mPlik, mKlasy, mKlasyfikator, mInfo, mGlowne;
	static int mklasa = 200;
	//static HWND uklad;
    	
/* PONI¯SZA INSTRUKCJA DEFINIUJE REAKCJE APLIKACJI NA POSZCZEGÓLNE MELDUNKI */
	switch (kod_meldunku) 
	{
	case WM_CREATE:  //meldunek wysy³any w momencie tworzenia okna

		mPlik = CreateMenu();
		AppendMenu(mPlik, MF_STRING, 100, "&Nowy");
		AppendMenu(mPlik, MF_STRING, 101, "&Koniec");

		mKlasy = CreateMenu();
		AppendMenu(mKlasy, MF_STRING, 200, "&Czerwona");
		AppendMenu(mKlasy, MF_STRING, 201, "&Zielona");
		AppendMenu(mKlasy, MF_STRING, 202, "&Niebieska");
		AppendMenu(mKlasy, MF_STRING, 203, "¯ó³&ta");

		mKlasyfikator = CreateMenu();
		AppendMenu(mKlasyfikator, MF_STRING, 300, "&Start!");
		AppendMenu(mKlasyfikator, MF_STRING, 301, "&Modu³owa!");
		AppendMenu(mKlasyfikator, MF_STRING, 302, "&Czyœæ");

		mInfo = CreateMenu();
		AppendMenu(mInfo, MF_STRING, 400, "&Autor...");

		mGlowne = CreateMenu();
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mPlik, "&Plik");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mKlasy, "&Klasy");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mKlasyfikator, "&Klasyfikator");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mInfo, "&Informacja");
		SetMenu(okno, mGlowne);
		DrawMenuBar(okno);

		CheckMenuItem(mKlasy, mklasa, MF_CHECKED);

		
		return 0;

	case WM_COMMAND:					//reakcje na wybór opcji z menu
		switch (wParam)
		{
		case 100: licz1 = licz2 = licz3 = licz0 = 0;	// Plik - > Nowy
				  InvalidateRect(okno, NULL, TRUE);
				  break;

		case 101: DestroyWindow(okno);	//Plik -> Koniec (wysylamy meldunek WM_DESTROY)
        		  break;	
		case 200:						//Klasy -> Czerwona			
				{
					CheckMenuItem(mKlasy, mklasa, MF_UNCHECKED);
					mklasa = 200;
					CheckMenuItem(mKlasy, mklasa, MF_CHECKED);
					aktKlasa = 0;
					break;
				}
		case 201:						//Klasy -> Zielona
				{
					CheckMenuItem(mKlasy, mklasa, MF_UNCHECKED);
					mklasa = 201;
					CheckMenuItem(mKlasy, mklasa, MF_CHECKED);
					aktKlasa = 1;
					break;
				}
		case 202:						//Klasy -> Niebieska
				{
					CheckMenuItem(mKlasy, mklasa, MF_UNCHECKED);
					mklasa = 202;
					CheckMenuItem(mKlasy, mklasa, MF_CHECKED);
					aktKlasa = 2;
					break;
				}
		case 203:						//Klasy -> ¯ó³ta
				{
					CheckMenuItem(mKlasy, mklasa, MF_UNCHECKED);
					mklasa = 203;
					CheckMenuItem(mKlasy, mklasa, MF_CHECKED);
					aktKlasa = 3;
					break;
				}
		case 300:						//Klasyfikator -> Start!
				{		
					HDC kontekst = GetDC(okno);
					COLORREF kolor = RGB(0, 0, 0);
					DPOINT tmpMidPoint;
					POINT tmpPt;

					struct vectorKlasa
					{
						DPOINT* klasa;
						DPOINT moda;
						int licz;
						COLORREF kolor;
					};

					vector<vectorKlasa> tmpVektorKlasa;
					tmpVektorKlasa.push_back({ klasa0, {0,0}, licz0, RGB(255, 0, 0) });
					tmpVektorKlasa.push_back({ klasa1, {0,0}, licz1, RGB(0, 255, 0) });
					tmpVektorKlasa.push_back({ klasa2, {0,0}, licz2, RGB(0, 0 ,255) });
					tmpVektorKlasa.push_back({ klasa3, {0,0}, licz3, RGB(255, 255, 0) });

					if (licz0 > 0) {
						tmpVektorKlasa[0].moda = WyszukajSrodek(klasa0, licz0);
						POINT pt0 = UkladNaOkno(tmpVektorKlasa[0].moda);
						RysujPunkt(kontekst, pt0, kolor);
					}

					if (licz1 > 0) {
						tmpVektorKlasa[1].moda = WyszukajSrodek(klasa1, licz1);
						POINT pt1 = UkladNaOkno(tmpVektorKlasa[1].moda);
						RysujPunkt(kontekst, pt1, kolor);
					}
					
					if (licz2 > 0) {
						tmpVektorKlasa[2].moda = WyszukajSrodek(klasa2, licz2);
						POINT pt2 = UkladNaOkno(tmpVektorKlasa[2].moda);
						RysujPunkt(kontekst, pt2, kolor);
					}

					if (licz3 > 0) {
						tmpVektorKlasa[3].moda = WyszukajSrodek(klasa3, licz3);
						POINT pt3 = UkladNaOkno(tmpVektorKlasa[3].moda);
						RysujPunkt(kontekst, pt3, kolor);
					}

					// krok 005
					// Funkcje OknoNaUklad oraz UkladNaOkno przeliczaj¹ wspó³rzêdne ekranowe na wspó³rzêdne uk³adu i odwrotnie.
					// W uk³adzie wspó³rzêdnych, w którym dzia³a klasyfikator, wartoœci x zmieniaj¹ siê w zakresie(-4; 4), a y(-3; 3).

					for (double x = -4; x <= 4; x += 0.05) {
						for (double y = -3; y <= 3; y += 0.05) {
							vector<double> dystans = { LDBL_MAX, LDBL_MAX, LDBL_MAX, LDBL_MAX };
							tmpMidPoint.x = x;
							tmpMidPoint.y = y;
							int tmpdystansmin = 99999;
							int tmpMinDystansKlasa = 99999;

							for (int klasa = 0; klasa < 4; klasa++)
							{
								
								if (tmpVektorKlasa[klasa].licz != 0)
								{
									dystans[klasa] = pow(x - tmpVektorKlasa[klasa].moda.x, 2) + pow(y - tmpVektorKlasa[klasa].moda.y, 2);
									if (tmpdystansmin > dystans[klasa]) {
										tmpdystansmin = dystans[klasa];
										tmpMinDystansKlasa = klasa;
									}
								
								}
							}	
							
							tmpPt = UkladNaOkno(tmpMidPoint);

							RysujPixel(kontekst, tmpPt, tmpVektorKlasa[tmpMinDystansKlasa].kolor);

						}
					}
					break;
				}

		case 301:						//Klasyfikator -> Czyœæ
				{
					HDC kontekst = GetDC(okno);
					COLORREF kolor = RGB(0, 0, 0);
					DPOINT tmpMidPoint;
					POINT tmpPt;

					struct vectorKlasa
					{
						DPOINT* klasa;
						DPOINT moda;
						int licz;
						COLORREF kolor;
					};

					vector<vectorKlasa> tmpVektorKlasa;
					tmpVektorKlasa.push_back({ klasa0, {0,0}, licz0, RGB(255, 0, 0) });
					tmpVektorKlasa.push_back({ klasa1, {0,0}, licz1, RGB(0, 255, 0) });
					tmpVektorKlasa.push_back({ klasa2, {0,0}, licz2, RGB(0, 0 ,255) });
					tmpVektorKlasa.push_back({ klasa3, {0,0}, licz3, RGB(255, 255, 0) });

					if (licz0 > 0) {
						tmpVektorKlasa[0].moda = WyszukajSrodek(klasa0, licz0);
						POINT pt0 = UkladNaOkno(tmpVektorKlasa[0].moda);
						RysujPunkt(kontekst, pt0, kolor);
					}

					if (licz1 > 0) {
						tmpVektorKlasa[1].moda = WyszukajSrodek(klasa1, licz1);
						POINT pt1 = UkladNaOkno(tmpVektorKlasa[1].moda);
						RysujPunkt(kontekst, pt1, kolor);
					}

					if (licz2 > 0) {
						tmpVektorKlasa[2].moda = WyszukajSrodek(klasa2, licz2);
						POINT pt2 = UkladNaOkno(tmpVektorKlasa[2].moda);
						RysujPunkt(kontekst, pt2, kolor);
					}

					if (licz3 > 0) {
						tmpVektorKlasa[3].moda = WyszukajSrodek(klasa3, licz3);
						POINT pt3 = UkladNaOkno(tmpVektorKlasa[3].moda);
						RysujPunkt(kontekst, pt3, kolor);
					}

					// krok 005
					// Funkcje OknoNaUklad oraz UkladNaOkno przeliczaj¹ wspó³rzêdne ekranowe na wspó³rzêdne uk³adu i odwrotnie.
					// W uk³adzie wspó³rzêdnych, w którym dzia³a klasyfikator, wartoœci x zmieniaj¹ siê w zakresie(-4; 4), a y(-3; 3).

					for (double x = -4; x <= 4; x += 0.05) {
						for (double y = -3; y <= 3; y += 0.05) {
							vector<double> dystans = { LDBL_MAX, LDBL_MAX, LDBL_MAX, LDBL_MAX };
							tmpMidPoint.x = x;
							tmpMidPoint.y = y;
							int tmpdystansmin = 99999;
							int tmpMinDystansKlasa = 99999;

							for (int klasa = 0; klasa < 4; klasa++)
							{

								if (tmpVektorKlasa[klasa].licz != 0)
								{
									dystans[klasa] = fabs(x - tmpVektorKlasa[klasa].moda.x) + fabs(y - tmpVektorKlasa[klasa].moda.y);
									if (tmpdystansmin > dystans[klasa]) {
										tmpdystansmin = dystans[klasa];
										tmpMinDystansKlasa = klasa;
									}
								}
							}

							tmpPt = UkladNaOkno(tmpMidPoint);

							RysujPixel(kontekst, tmpPt, tmpVektorKlasa[tmpMinDystansKlasa].kolor);
						}
					}
					break;
				}
		case 302:						//Klasyfikator -> Czyœæ

				break;

		case 400: MessageBox(okno, "Imiê i nazwisko: Artur Tryc\nNumer indeksu: 142248", "Autor", MB_OK);
		}
		return 0;
	
	case WM_LBUTTONDOWN:				//reakcja na lewy przycisk myszki
		{	
			POINT ptOkna;

			int x = ptOkna.x = LOWORD(lParam);
			int y = ptOkna.y = HIWORD(lParam);

			if ((x > 50)&&(x<650)&&(y>50)&&(y<500))
			{
				HDC kontekst = GetDC(okno);
				COLORREF kolor;

				switch (aktKlasa)
				{
				case 0:
					kolor = RGB(255, 0, 0);
					if (licz0 < 100)
					{
						klasa0[licz0] = OknoNaUklad(ptOkna);
						licz0++;
						RysujPunkt(kontekst, ptOkna, kolor);
					};
					break;

				case 1:
					kolor = RGB(0, 255, 0);
					if (licz1 < 100)
					{
						klasa1[licz1] = OknoNaUklad(ptOkna);
						licz1++;
						RysujPunkt(kontekst, ptOkna, kolor);
					};
					break;

				case 2:
					kolor = RGB(0, 0, 255);
					if (licz2 < 100)
					{
						klasa2[licz2] = OknoNaUklad(ptOkna);
						licz2++;
						RysujPunkt(kontekst, ptOkna, kolor);
					};
					break;
	
				case 3:
					kolor = RGB(255, 255, 0);
					if (licz3 < 100)
					{
						klasa3[licz3] = OknoNaUklad(ptOkna);
						licz3++;
						RysujPunkt(kontekst, ptOkna, kolor);
					};
					break;
				}
				
				ReleaseDC(okno, kontekst);
			}

			return 0;
		}
	
	case WM_PAINT:						//obs³uga operacji graficznych
		{
			PAINTSTRUCT paint;
			HDC kontekst;

			kontekst = BeginPaint(okno, &paint);

			RysujUklad(kontekst);
			RysujPunkty(kontekst);


			EndPaint(okno, &paint);
			
			return 0;
		}
  	
	case WM_DESTROY:					//obowi¹zkowa obs³uga meldunku o zamkniêciu okna
		PostQuitMessage (0) ;
		return 0;
    
	default:							//standardowa obs³uga pozosta³ych meldunków
		return DefWindowProc(okno, kod_meldunku, wParam, lParam);
	}
}

BOOL RysujUklad(HDC kontekst)
{
	BOOL bWyn = TRUE;

	HPEN pioro = CreatePen(PS_SOLID, 2, RGB(0,0,0));
	SelectObject(kontekst, pioro);

						
	MoveToEx(kontekst, 350, 50, NULL);
	LineTo(kontekst, 350, 500);

	MoveToEx(kontekst, 350, 50, NULL);
	LineTo(kontekst, 345, 60);

	MoveToEx(kontekst, 350, 50, NULL);
	LineTo(kontekst, 354, 60);
	

	MoveToEx(kontekst, 50, 275, NULL);
	LineTo(kontekst, 650, 275);

	MoveToEx(kontekst, 650, 275, NULL);
	LineTo(kontekst, 640, 270);

	MoveToEx(kontekst, 650, 275, NULL);
	LineTo(kontekst, 640, 280);


	MoveToEx(kontekst, 425, 270, NULL);
	LineTo(kontekst, 425, 280);
	MoveToEx(kontekst, 500, 270, NULL);
	LineTo(kontekst, 500, 280);
	MoveToEx(kontekst, 575, 270, NULL);
	LineTo(kontekst, 575, 280);
	MoveToEx(kontekst, 275, 270, NULL);
	LineTo(kontekst, 275, 280);
	MoveToEx(kontekst, 200, 270, NULL);
	LineTo(kontekst, 200, 280);
	MoveToEx(kontekst, 125, 270, NULL);
	LineTo(kontekst, 125, 280);

	MoveToEx(kontekst, 344, 350, NULL);
	LineTo(kontekst, 355, 350);
	MoveToEx(kontekst, 344, 425, NULL);
	LineTo(kontekst, 355, 425);
	MoveToEx(kontekst, 345, 200, NULL);
	LineTo(kontekst, 355, 200);
	MoveToEx(kontekst, 345, 125, NULL);
	LineTo(kontekst, 355, 125);

	DeleteObject(pioro);

	return bWyn;
}

DPOINT OknoNaUklad(POINT ptOkna)
{
	DPOINT ptWyn;

	ptWyn.x = (double)(ptOkna.x - 50 - 300)/75.0;
	ptWyn.y = (double)(ptOkna.y - 275)/-75.0;

	return ptWyn;
}

POINT UkladNaOkno(DPOINT ptUkladu)
{
	POINT ptWyn;

	ptWyn.x = (long)(ptUkladu.x*75 + 50 + 300);
	ptWyn.y = (long)(-ptUkladu.y*75 + 275);

	return ptWyn;
}

BOOL RysujPunkt(HDC kontekst, POINT pt, COLORREF kolor)
{
	BOOL bWyn = TRUE;

	HPEN pioro = CreatePen(PS_SOLID, 2, kolor);
	SelectObject(kontekst, pioro);

	Ellipse(kontekst, pt.x-3, pt.y-3, pt.x+3, pt.y+3);

	DeleteObject(pioro);
	
	return bWyn;
}

BOOL RysujPunkty(HDC kontekst)
{
	BOOL bWyn = TRUE;

	int i;

	for (i = 0; i < licz0; i++)
	{
		POINT pt = UkladNaOkno(klasa0[i]);
		RysujPunkt(kontekst, pt, RGB(255,0,0));
	}

	for (i = 0; i < licz1; i++)
	{
		POINT pt = UkladNaOkno(klasa1[i]);
		RysujPunkt(kontekst, pt, RGB(0,255,0));
	}

	for (i = 0; i < licz2; i++)
	{
		POINT pt = UkladNaOkno(klasa2[i]);
		RysujPunkt(kontekst, pt, RGB(0,0,255));
	}

	for (i = 0; i < licz3; i++)
	{
		POINT pt = UkladNaOkno(klasa3[i]);
		RysujPunkt(kontekst, pt, RGB(255,255,0));
	}
	
	return bWyn;
}

DPOINT WyszukajSrodek(DPOINT __klasa[], int __licz) {
	DPOINT midPoint;
	midPoint.x = 0;
	midPoint.y = 0;

	for (int i = 0; i < __licz; i++)
	{
		midPoint.x += __klasa[i].x;
		midPoint.y += __klasa[i].y;
	}
	midPoint.x /= __licz;
	midPoint.y /= __licz;

	return midPoint;
}

BOOL RysujPixel(HDC kontekst, POINT pt, COLORREF kolor)
{
	BOOL bWyn = TRUE;

	HPEN pioro = CreatePen(PS_SOLID, 1, kolor);
	SelectObject(kontekst, pioro);

	Ellipse(kontekst, pt.x - 1, pt.y - 1, pt.x + 1, pt.y + 1);

	DeleteObject(pioro);

	return bWyn;
}


/*	ZADANIA
Program nale¿y uzupe³niæ o implementacjê klasyfikatora minimalnoodleg³oœciowego z zadanymi metrykami.

Klasyfikator powinien byæ uruchamiany po wybraniu z menu opcji Klasyfikator -> Start (wywo³anie w linii 168). 
Efektem jego dzia³ania ma byæ odpowiednie pokolorowanie uk³adu wspó³rzêdnych, tak aby widoczne by³y 
obszary decyzyjne odpowiadaj¹ce poszczególnym klasom. Dodatkowo, dla ka¿dej klasy nale¿y zaznaczyæ wartoœæ œredni¹ (modê).

Opcja Klasyfikator -> Czyœæ (wywo³anie w linii 171) ma s³u¿yæ do usuwania obszarów decyzyjnych z ekranu, jednak 
przy zachowaniu wprowadzonych punktów. 

	DODATKOWE INFORMACJE
Wspó³rzêdne wprowadzonych punktów s¹ przechowywane w tablicach klasa0, klasa1, klasa2, klasa3. S¹ to tablice typu
DPOINT (dwa pola, 'x' i 'y', typu double). Liczba punktów ka¿dej z klas zawarta jest w zmiennych licz0, licz1, licz2,
licz3.

Funkcje OknoNaUklad oraz UkladNaOkno przeliczaj¹ wspó³rzêdne ekranowe na wspó³rzêdne uk³adu i odwrotnie.
W uk³adzie wspó³rzêdnych, w którym dzia³a klasyfikator, wartoœci x zmieniaj¹ siê w zakresie (-4; 4), a y (-3; 3).

Funkcja RysujPunkt rysuje na ekranie okr¹g o promieniu 3 pikseli w zadanym kolorze - np. RGB(255,0,0).
*/