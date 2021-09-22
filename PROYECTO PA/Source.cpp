#include <Windows.h>
#include <string>
#include <fstream>
#include "resource.h"
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <oleauto.h>
#include <commctrl.h>
#include <sstream>
#define MI_TIMER_GENERADO 117
time_t actualtime;
tm* timeInfo;
using namespace std;
string fecha;

struct User {
	int IDUser;
	string username;
	string password;
	User* next;
	User* prev;
	int T;
}*oUser, *aUser, * userAcces;
int k = 1;
int idus;
struct Uinfo {
	int IDUser;
	string Nombrecom;
	string Aliascom;
	string imagencom;
	Uinfo*next;
	Uinfo* prev;
}*oInf, *aInf, *userInfo,*cam;
char cDirFile[MAX_PATH] = "";
string rutatemp;
string prodtemp;
struct Productos {
	int IDProd; //id quue debe de ser el mismo que el de la persona que lo dio de alta 
	int IDUser;
	string *Cambio;
	string Pnombre;
	string Pinv;
	string Pcodig;
	string Pmarca;
	string Pdescr;
	string PMonto;
	char imagen[MAX_PATH];
	Productos*next;
	Productos* prev;
}*oProd, *aProd, *cProd;

struct Envios {
	int IDUser;
	string Enprod;
	string cantidad;
	string monto;
	string calle;
	string colonia;
	string ciudad;
	string estado;
	string mensaje;
	string fecha;
	Envios*next;
	Envios*prev;
}*oEnvios, *aEnvios;
char Mostrar [50];
HWND hlblClientes;
HWND hLblProductos;
HWND hTxtInfoNombre;
HWND hTxtInfoEmpresa;
HWND hTxtInfoId;

HWND hTxtSelectedNombre;
HWND hTxtSelectedCodigo;
HWND hTxtSelectedCantidad;
HWND hTxtSelectedMarca;
HWND hTxtSelectedDescripcion;
HWND hTxtSelectedMonto;
bool searchFound;
bool exitProgram = false;
bool exitProgramRegister = false;
HINSTANCE hGlobalInst;
HWND hLBLnombre;
HWND hLBLempresa;
HWND hid;
HWND hTxtRegisterUsername;
HWND hTxtRegisterPassword;
HWND hTxtLoginUsername;
HWND hTxtLoginPassword;
HWND hTxtInfVendedor;
HWND hTxtInfAlias;
HWND hTxtProd_Nom;
HWND hTxtProd_Inv;
HWND hTxtProd_Cod;
HWND hTxtProd_Marca;
HWND hTxtProd_Desc;
HWND hTxtProd_Monto;
HWND hTxtEnvios;
HWND hTxtCodigo;
HWND hTxtprodbus;
HMENU hMenuOpciones;
fstream LectorEscritor; 
int GLOBAL_USER_ID=1;

void loadUser();
void save(User*);
void saveInfo(Uinfo*);
void saveenvios(Envios*);
void loadInfo();
void freeMemoryProd();
void freeMemoryUser();
void freeMemoryInfo();
void getGlobalid();//oobtener 1 registro del numero a guardar
void saveGlobalid();//guardar el numero en archivo binario
void saveProd(Productos*);
void loadProd();
void saveEnvios(Envios*);
void loadEnvios();
string getText(HWND);

BOOL CALLBACK fPrincipal(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fRegister(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK flogin(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fInfo(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fInfoVendedor(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fcamInfo(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK falta(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fbaja(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fcam(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fcambios(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fcambioss(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fNEnvio(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fProductos(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {
	getGlobalid();
	oUser = aUser = NULL;
	hGlobalInst = hInst;
	
	HWND hlogin = CreateDialog(hInst, MAKEINTRESOURCE(DLG_LOGIN), NULL,flogin );//puntero a una ventana 
	
	ShowWindow(hlogin, cShow);//muestra una ventana.

	MSG msg; //estructura que captura a todos los eventos/mensajes que le sucedan a las ventanas.
	ZeroMemory(&msg, sizeof(MSG));//prellenamos todos los valores y variables de MSG con 0.

	//GetMessage();//obtiene un mensaje/ evento que le ocurrio a la app
	//devuelve true para todos los ventos excepto 1(WM_QUIT)
	// el WM_QUIT SE MANDA A LLAMAR CON POSTQUITMESSAGE(0);
	//en getmessage para escuchar todas las ventanas debemos usar NULL

	while (GetMessage(&msg,NULL,0,0)) {
		TranslateMessage(&msg);// convierte de lenguaje maquina a las constantes enteras de WM_MENSAJE
		DispatchMessage(&msg);//ENVIA EL EVENTO A LA VENTANA QUE SUFRIO EL CAMBIO
	}
	//msg.wParam; //almacena el int que regreso postquitmessage
	return msg.wParam;
}

BOOL CALLBACK flogin(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) { //ventana de login
	
	switch (msg) {
	case WM_INITDIALOG:{

		loadUser();
		
		hTxtLoginUsername = GetDlgItem(hwnd, TXT_LUSER);
		hTxtLoginPassword = GetDlgItem(hwnd, TXT_LPASS);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_Reg: {
			HWND hRegister = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_REG), NULL, fRegister);
			ShowWindow(hRegister, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
		}break;
		  case BTN_LOG: {
			  string userName = getText(hTxtLoginUsername);
			  string pasSword = getText(hTxtLoginPassword);
			  if (userName.compare("") == 0 || pasSword.compare("") == 0) {
				  MessageBox(NULL, "Falto llenar la informacion de usuario","", MB_ICONASTERISK);
				  break;
			  }
			  
			  if (oUser != NULL) {
				  bool found = true;
				  while (aUser != NULL) {
					  if (aUser->username.compare(userName)==0 && aUser->password.compare(pasSword)==0) {
						  found = true;
						  break;
					  }
					  if(aUser->next != NULL){
						  found = false;
					  aUser = aUser->next;
                      }
				  }
				  if (found == true) {
					  
					  userAccess = aUser;
                      aUser = oUser;
					  if (userAccess->T == k) {// validacion para ver que entre a la primera o segunda pantalla
						  userAccess->T = 0;
						  
						  HWND hinfo = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_INF), NULL, fInfo);
						  ShowWindow(hinfo, SW_SHOW);
						  DestroyWindow(hwnd);
						  break;
					  }
					  else if (userAccess->T!=k) {
						  loadInfo();

					  HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
					  loadProd();
					  loadInfo();
					  loadEnvios();
					  hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
					  SetMenu(hprincipal, hMenuOpciones);
					  ShowWindow(hprincipal, SW_SHOW);
					  exitProgram = false;
					  DestroyWindow(hwnd);
					  break;
				      }
				  }
				  else
					  aUser = oUser;
			  }
			  else {
				  MessageBox(NULL, "Ningun usuario coincide con las credenciales", "No Login ", MB_ICONASTERISK);
				  break;
			  }
			
			
		  }break;
	    }
	}break;
	case WM_CLOSE://Evento que se manda a llamar al darle click al boton X
		exitProgram = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY://Evento que se manda a llamar al ejecutar la funcion DestroyWindow
		if (exitProgram) 
		PostQuitMessage(9);
		break;

	
	}
	return FALSE;
}

BOOL CALLBACK fRegister(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) { //ventana de registro
	switch (msg) {
	case WM_INITDIALOG: {
		
		 hTxtRegisterUsername = GetDlgItem(hwnd,TXT_NUSER );
		 hTxtRegisterPassword = GetDlgItem(hwnd,TXT_NPASSWORD);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		  case BTN_NUSER: {
				string userName = getText(hTxtRegisterUsername);
				string pasSword = getText(hTxtRegisterPassword);
				if (userName.compare("") == 0 || pasSword.compare("") == 0) {
					MessageBox(NULL, "Falto llenar la informacion de usuario", "No procede la alta", MB_ICONASTERISK);
					break;
				}
				if (oUser != NULL) {
					bool found = true;
					while (aUser->username.compare(userName) != 0 ) {
						if (aUser->next==NULL) {
							
							found = false;
							break;
						}
						aUser = aUser->next;
					}
					if (found == true) {
						MessageBox(NULL, "Ya existe un usuario con estas credenciales", "No procede la alta", MB_ICONASTERISK);
						MessageBox(NULL, "Ingrese otro nombre de usuario", " ", MB_ICONASTERISK);
						aUser = oUser;
						break;
					}
					else
						aUser = oUser;
				}
				if (oUser == NULL) {
					oUser = new User;
					oUser->IDUser = GLOBAL_USER_ID;
					oUser->username.append(userName);
					oUser->password.append(pasSword);
					oUser->T = 1;
					oUser->next = NULL;
					oUser->prev = NULL;
					aUser = oUser;
					GLOBAL_USER_ID++;
				}
				else {
					while (aUser->next != NULL)
						aUser = aUser->next;
					aUser->next = new User;
					aUser->next->prev = aUser;
					aUser = aUser->next;
					aUser->IDUser = GLOBAL_USER_ID;
					aUser->username.append(userName);
					aUser->password.append(pasSword);
					aUser->T = 1;
					aUser->next = NULL;
					aUser = oUser;
					GLOBAL_USER_ID++;
				}
				save(aUser);
				saveGlobalid();
				aUser = oUser;
				freeMemoryUser();
				HWND hlogin = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_LOGIN), NULL, flogin);
				ShowWindow(hlogin, SW_SHOW);
				exitProgramRegister = false;
				DestroyWindow(hwnd);
		  }break;

		  case BTN_RCAN: {
			  HWND hlogin = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_LOGIN), NULL, flogin);
			  ShowWindow(hlogin, SW_SHOW);
			  exitProgramRegister = false;
			  DestroyWindow(hwnd);
		  }
		}
	}break;
	//case WM_CLOSE://Evento que se manda a llamar al darle click al boton X
	//exitProgramRegister = true;
	//DestroyWindow(hwnd);
	//	break; lo tengo asi para que no sirva la X en la parte de registro 
	case WM_DESTROY:
	if(exitProgramRegister)
	PostQuitMessage(9);
		break;

	}
	return FALSE;
}

BOOL CALLBACK fPrincipal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {//ventana principal, donde se muestran los envios
	
	switch (msg) {
	case WM_INITDIALOG: {
		

		hTxtSelectedNombre = GetDlgItem(hwnd, TXT_ENVIOS_DIRECCION);
		hTxtSelectedCodigo = GetDlgItem(hwnd, TXT_ENVIOS_FECHA);
		hTxtSelectedCantidad = GetDlgItem(hwnd, TXT_ENVIOS_PRODUCTO);
		hTxtSelectedMarca = GetDlgItem(hwnd, TXT_ENVIOS_CANTIDAD);
		hTxtSelectedDescripcion = GetDlgItem(hwnd, TXT_ENVIOS_ESTATUS);
		hTxtSelectedMonto = GetDlgItem(hwnd, TXT_PROD_MONTO_SELECTED);
		hLblProductos = GetDlgItem(hwnd, LB_ENVIOS);
		int index = 0;
		while (aEnvios != NULL) {
			SendMessage(hLblProductos, LB_ADDSTRING, NULL, (LPARAM)aEnvios->Enprod.c_str());
			SendMessage(hLblProductos, LB_SETITEMDATA, (WPARAM)index, (LPARAM)aEnvios->IDUser);
			aEnvios = aEnvios->next;

		}
		aEnvios = oEnvios;


	}break;
	case WM_COMMAND: {
		if (LOWORD(wparam) == LB_ENVIOS && HIWORD(wparam) == LBN_SELCHANGE) {
			int index = SendMessage(hLblProductos, LB_GETCURSEL, NULL, NULL);
			int IdSelectedProd = SendMessage(hLblProductos, LB_GETITEMDATA, (WPARAM)index, NULL);
			bool found = true;
			while (aEnvios != NULL) {

				if (aEnvios->IDUser == IdSelectedProd)
					break;
				if (aEnvios->next == NULL) {
					found = false;
					break;
				}
				aEnvios = aEnvios->next;
			}
			if (!found)
				aEnvios = oEnvios;
			else {
				char cCod[10];
				//char cCant[100];
				//_itoa(aProd->Pcodig, cCod, 100);
				//_itoa(aProd->Pinv, cCant, 100);
				SetWindowText(hTxtSelectedNombre, aEnvios->ciudad.c_str());
				SetWindowText(hTxtSelectedCantidad, aEnvios->Enprod.c_str());
				SetWindowText(hTxtSelectedCodigo, aEnvios->fecha.c_str());
				SetWindowText(hTxtSelectedMarca, aEnvios->cantidad.c_str());
				SetWindowText(hTxtSelectedDescripcion, aEnvios->estado.c_str());
				
				aEnvios = oEnvios;
			}
		}
		   switch (LOWORD(wparam)) {
		   case ID_INFORMACIONDELVENDEDOR: {
			   HWND hInfoVendedor = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_Info_vendedor), NULL, fInfoVendedor);
			   ShowWindow(hInfoVendedor, SW_SHOW);
			   exitProgram = false;
			   DestroyWindow(hwnd);
			   break;
		   }
		case ID_PRODUCTOS_ALTA: {
			HWND halta = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_Alta_producto), NULL, falta);
			ShowWindow(halta, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
		}break;
		case ID_ENVIOS_COMPRARPRODUCTOS: {
			HWND hEnvio = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_NEW_ENVIO), NULL, fNEnvio);
			SetTimer(hEnvio, MI_TIMER_GENERADO,1000,(TIMERPROC)NULL);
			ShowWindow(hEnvio, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
		}break;
		case ID_PRODUCTOS_BAJA: {
			HWND hbaja = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_Baja_Producto), NULL, fbaja);
			ShowWindow(hbaja, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
		}break;
		case ID_PRODUCTOS_CAMBIO: {
			HWND hcam = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_Cambio_producto), NULL, fcam);
			ShowWindow(hcam, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
		}break;
		case ID_PRODUCTOS_MISPRODUCTOS: {
			HWND hProductos = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_MIS_PRODUCTOS), NULL, fProductos);
			ShowWindow(hProductos, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
		}break;
		case ID_ENVIOS_CANCELARUNENVIO :{

		}break;
		case ID_ENVIOS_EDITARUNENVIO: {

		}break;
		case ID_SALIR: {
		case WM_CLOSE://Evento que se manda a llamar al darle click al boton X
			exitProgram = true;
			DestroyWindow(hwnd);
			break;
		}
		

		   }
	}break;
	
	case WM_DESTROY://Evento que se manda a llamar al ejecutar la funcion DestroyWindow
		if (exitProgram)
			PostQuitMessage(9);
		break;

	}
	return FALSE;
}

BOOL CALLBACK fInfo(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) { //ventana donde recibe la informacion de nuevos usuarios
	switch (msg) {
	case WM_INITDIALOG: {
		loadInfo();
		hTxtInfVendedor = GetDlgItem(hwnd, TXT_INF_VENDEDOR);
		hTxtInfAlias = GetDlgItem(hwnd, TXT_INF_ALIAS);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_INFO_IMAGEN: {
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				
				ofn.hwndOwner = hwnd;
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.lpstrFile = cDirFile;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrDefExt = "txt";
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrFilter = "Imagenes bmp\0*.bmp\0" ;

				if (GetOpenFileName(&ofn)) {
				HBITMAP hbimagen = (HBITMAP)LoadImage(NULL, cDirFile, IMAGE_BITMAP,400,600,LR_LOADFROMFILE);
				HWND hPbImagen = GetDlgItem(hwnd, PB_IMAGEN);
				SendMessage(hPbImagen,STM_SETIMAGE, IMAGE_BITMAP,(LPARAM) hbimagen);
				rutatemp = cDirFile;
				}

		}break;
		case BTN_INF_ACEP: {
			string Name = getText(hTxtInfVendedor);
			string Alias = getText(hTxtInfAlias);
			if (Name.compare("") == 0 || Alias.compare("") == 0) {
				MessageBox(NULL, "Falto llenar la informacion de usuario", " ", MB_ICONASTERISK);
				break;
			}
			if (oInf != NULL) {
				bool found = true;
				while (aInf->Nombrecom.compare(Name) != 0) {
					if (aInf->next == NULL) {

						found = false;
						break;
					}
					aInf = aInf->next;
				}
				if (found == true) {
					MessageBox(NULL, "Ya existe un usuario con estas credenciales", "No procede la alta", MB_ICONASTERISK);
					MessageBox(NULL, "Ingrese otro nombre de usuario", " ", MB_ICONASTERISK);
					aUser = oUser;
					break;
				}
				else
					aUser = oUser;
			}
			if (oInf == NULL) {
				oInf = new Uinfo;
				oInf->Nombrecom.append(Name);
				oInf->Aliascom.append(Alias);
				oInf->IDUser = userAccess->IDUser;
				/*oInf->imagencom.append(rutatemp);*/
				oInf->next = NULL;
				oInf->prev = NULL;
				aInf = oInf;
			}
			else {
				while (aInf->next != NULL)
					aInf = aInf->next;
				aInf->next = new Uinfo;
				aInf->next->prev = aInf;
				aInf = aInf->next;
				aInf->Nombrecom.append(Name);
				aInf->Aliascom.append(Alias);
				aInf->IDUser = userAccess->IDUser;
				/* aInf->imagencom.append(rutatemp);*/
				aInf->next = NULL;
				aInf = oInf;
			}
			saveInfo(aInf);
			save(aUser);
			aInf = oInf;
			freeMemoryInfo();
			loadInfo();

			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
		}break;
		}
	}
	//case WM_CLOSE://Evento que se manda a llamar al darle click al boton X
	//	exitProgram = false;
	//	DestroyWindow(hwnd);
	//	break;
	case WM_DESTROY://Evento que se manda a llamar al ejecutar la funcion DestroyWindow
		if (exitProgram)
			PostQuitMessage(9);
		break;

	}
	return FALSE;
}
BOOL CALLBACK falta(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {//ventana de alta de producto
	switch (msg) {
	case WM_INITDIALOG: {

		hTxtProd_Nom = GetDlgItem(hwnd, TXT_PROD_NOM);
		hTxtProd_Inv = GetDlgItem(hwnd, TXT_PROD_INV);
	    hTxtProd_Cod = GetDlgItem(hwnd, TXT_PROD_COD);
		hTxtProd_Marca = GetDlgItem(hwnd, TXT_PROD_MARCA);
		hTxtProd_Desc = GetDlgItem(hwnd, TXT_PROD_DESC);
		hTxtProd_Monto = GetDlgItem(hwnd, TXT_PROD_MONTO);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_NPROD: {
			string prodnom = getText(hTxtProd_Nom);
			string prodinv = getText(hTxtProd_Inv);
			string prodcod = getText(hTxtProd_Cod);
			string prodmar = getText(hTxtProd_Marca);
			string proddesc = getText(hTxtProd_Desc);
			string prodmon = getText(hTxtProd_Monto);

			if (prodnom.compare("") == 0 || prodinv.compare("") == 0 || prodcod.compare("") == 0 || prodmar.compare("") == 0 || proddesc.compare("") == 0|| prodmon.compare("")==0) {
				MessageBox(NULL, "Falto llenar la informacion de Producto", "No procede la alta", MB_ICONASTERISK);
				break;
			}
			if (oProd != NULL) {
				bool found = true;
				while (aProd->Pnombre.compare(prodnom) != 0) {
					if (aProd->next == NULL) {

						found = false;
						break;
					}
					aProd = aProd->next;
				}
				if (found == true) {
					MessageBox(NULL, "Ya existe un producto con este nombre", "No procede la alta", MB_ICONASTERISK);
					MessageBox(NULL, "Ingrese otro nombre de producto", " ", MB_ICONASTERISK);
					aProd = oProd;
					break;
				}
				else
					aProd = oProd;
			}
			if (oProd == NULL) {
				oProd = new Productos;
				oProd->IDUser = userAccess->IDUser;
				oProd->Pnombre.append(prodnom);
				oProd->Pinv.append(prodinv);
				oProd->Pcodig.append(prodcod);
				oProd->Pmarca.append(prodmar);
				oProd->Pdescr.append(proddesc);
				oProd->PMonto.append(prodmon);
				
				oProd->next = NULL;
				oProd->prev = NULL;
				aProd = oProd;
				
			}
			else {
				while (aProd->next != NULL)
					aProd = aProd->next;
				aProd->next = new Productos;
				aProd->next->prev = aProd;
				aProd = aProd->next;
				aProd->IDUser = userAccess->IDUser;
				aProd->Pnombre.append(prodnom);
				aProd->Pinv.append(prodinv);
				aProd->Pcodig.append(prodcod);
				aProd->Pmarca.append(prodmar);
				aProd->Pdescr.append(proddesc);
				aProd->PMonto.append(prodmon);
				aProd->next = NULL;
				aProd = oProd;
				
			}
			saveProd(aProd);
			
			
			aProd = oProd;
			freeMemoryProd();
			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			loadProd();
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgramRegister = false;
			DestroyWindow(hwnd);
		}break;

		case BTN_CANCEL_ALTA: {
			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgramRegister = false;
			DestroyWindow(hwnd);
		}
		}
	}break;

	case WM_DESTROY://Evento que se manda a llamar al ejecutar la funcion DestroyWindow
		if (exitProgram)
			PostQuitMessage(9);
		break;
	}
	return FALSE;
}

BOOL CALLBACK fbaja(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {//vetana donde se dan de baja los productos
	switch (msg) {

	case WM_INITDIALOG: {
		hTxtCodigo = GetDlgItem(hwnd, TXT_CODIGO_BUSQUEDA);
		loadProd();
		//hlblClientes = GetDlgItem(hwnd, LIST_CLIENTE);
		///
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_ACP_BAJA: {
			string codigo = getText(hTxtCodigo);
			
			if (oProd == NULL) {//Lista Vacia
				MessageBox(NULL, "No hay elementos en la lista", "Lista vacia", MB_ICONASTERISK);

				break; //Reiniciar el switch
			}
			bool searchFound = true; //Vamos a obviar que lo encontramos
				while (aProd->Pcodig != codigo) { //NOS PERMITE RECORRER TODA LA LISTA
				if (aProd->next == NULL) { //Ultimo elemento, lo que quiere decir que no encontramos la matricula
					searchFound = false;
					break;
				}
				aProd = aProd->next;//Tomamos la direccion de memoria del siguiente usuario y se la asignamos a AUX para avanzar
			}
			if (searchFound == false) { //No encontramos al usuario
				
				MessageBox(NULL, "Producto no encontrado", "No encontrado", MB_ICONASTERISK);
				
				aProd = oProd; // Asignamos AUX al primer elemento (origen)
				break; //Reiniciar el switch
			}

			if (aProd->next == NULL && aProd->prev == NULL) { //UNICO
				strcpy(Mostrar, aProd->Pnombre.c_str());
				
				MessageBox(NULL, Mostrar, "El Producto Encontrado es:", MB_ICONQUESTION);
				
				int msgboxID = MessageBox(NULL, "Desea eliminar el producto?", "Eliminar producto",MB_ICONQUESTION | MB_OKCANCEL );
				switch (msgboxID) {
				case IDOK:{
					delete aProd; //Borro el unico elemento
					aProd = oProd;
					oProd = aProd = NULL; //Asigno origen y aux a NULL permitiendo que el if del modulo ALTA pueda entrar a la lista vacia
					
				}break;
				case IDCANCEL:// you check msdn for more cases
				{
					

				}break;
				
				}break;
				saveProd(oProd);
				freeMemoryProd();
			}
			else if (aProd->next == NULL) { //ULTIMO
				strcpy(Mostrar, aProd->Pnombre.c_str());

				MessageBox(NULL, Mostrar, "El Producto Encontrado es:", MB_ICONQUESTION);
				int msgboxID = MessageBox(NULL, "Desea eliminar el producto?", "Eliminar producto", MB_ICONQUESTION | MB_OKCANCEL);
				switch (msgboxID) {
				case IDOK: {
					aProd->prev->next = NULL; // De aux (ultimo elemento) viaje a prev (Penultimo elemento) accedi a su sig (puntero) lo iguale a NULL
					//Convirtiendo el penultimo elemento en el ultimo
					delete aProd; //Borre aux

					aProd = oProd; //Regrese aux al origen
					saveProd(aProd);
					freeMemoryProd();

				}
				case IDCANCEL:// you check msdn for more cases
				{
					saveProd(oProd);
						
				}break;
				}break;
			}
			else if (aProd->prev == NULL) { //PRIMERO
				strcpy(Mostrar, aProd->Pnombre.c_str());

				MessageBox(NULL, Mostrar, "El Producto Encontrado es:", MB_ICONQUESTION);
				int msgboxID = MessageBox(NULL, "Desea eliminar el producto?", "Eliminar producto", MB_ICONQUESTION | MB_OKCANCEL);
				switch (msgboxID) {
				case IDOK: {
					oProd = oProd->next; //Muevo origen el NUEVO INICIO de la lista, como voy a borrar el primero, el segundo elemento se convierte en el nuevo origen
					oProd->prev = NULL; //Al nuevo origen le asigno el previo a NULL para saber que es el inicio de la lista
					delete aProd;//Sin tener ataduras al origen borramos aux;

					aProd = oProd; //Regrese aux al origen
					saveProd(aProd);
					freeMemoryProd();
				}
				case IDCANCEL:// you check msdn for more cases
				{


				}break;
				}
			}
			else { //CUALQUIER OTRO
				strcpy(Mostrar, aProd->Pnombre.c_str());

				MessageBox(NULL, Mostrar, "El Producto Encontrado es:", MB_ICONQUESTION);
				int msgboxID = MessageBox(NULL, "Desea eliminar el producto?", "Eliminar producto", MB_ICONQUESTION | MB_OKCANCEL);
				switch (msgboxID) {
				case IDOK: {
					aProd->prev->next = aProd->next; // De aux(Nodo Actual) obtengo prev (Nodo anterior) de el obtengo el puntero(Sig) y lo asigno a aux->sig(Nodo siguiente)
					aProd->next->prev = aProd->prev;// De aux(Nodo Actual) obtengo sig (Nodo siguiente) de el obtengo el puntero(Prev) y lo asigno a aux->prev(Nodo Anterior)
					delete aProd; //Elimino aux

					aProd = oProd; //Regrese aux al origen
					saveProd(aProd);
					freeMemoryProd();
				}
				case IDCANCEL:// you check msdn for more cases
				{


				}break;
				}break;
			}
		}break;
		case BTN_BAJA_CANCEL: {
			
			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
		}break;
		}
	}
	}
	return 0;
}

BOOL CALLBACK fcam(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){//ventana donde se cambia el producto
	switch (msg) {
	case WM_INITDIALOG: {

		loadProd();
		hTxtprodbus = GetDlgItem(hwnd, TXT_PROD_Cam);
		
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case IDCANCEL: {
			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
		}break;
		case BTN_CAM_PROD: {
			string userName = getText(hTxtprodbus);
			
			if (userName.compare("") == 0) {
				MessageBox(NULL, "No escribio nada", "", MB_ICONASTERISK);
				break;
			}
			if (oProd != NULL) {
				bool found = true;
				while (aProd != NULL) {
					if (aProd->Pcodig.compare(userName )==0 && aProd->IDUser == userAccess->IDUser) {
						found = true;
						break;
					}
					if (aProd->next != NULL) {
						found = false;
						aProd = aProd->next;
					}
				}
				if (found == true) {

					cProd = aProd;
					aProd = oProd;
					
					HWND hcambios = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_Cambios_productos), NULL, fcambios);
					ShowWindow(hcambios, SW_SHOW);
					exitProgram = false;
					DestroyWindow(hwnd);
					
				}
				else
					aProd = oProd;
			}
			else {
				MessageBox(NULL, "Ningun Producto coincide con el codigo", "No Econtrado ", MB_ICONASTERISK);
				break;
			}


		}break;
		case BTN_CAN_CAM: {
			
			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
			break;
		}

		}
	}break;
	//case WM_CLOSE://Evento que se manda a llamar al darle click al boton X
	//	exitProgram = true;
	//	DestroyWindow(hwnd);
	//	break;
	case WM_DESTROY://Evento que se manda a llamar al ejecutar la funcion DestroyWindow
		if (exitProgram)
			PostQuitMessage(9);
		break;


	}
	return FALSE;
}

BOOL CALLBACK fNEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) { //ventana donde se realizan los envios 

	switch (msg) {
	case WM_INITDIALOG: {
		
		hTxtProd_Inv = GetDlgItem(hwnd, TXT_ENVIO_CANTIDAD);//CANTIDAD
		hTxtProd_Cod = GetDlgItem(hwnd, TXT_ENVIO_CALLE);//CALLE
		hTxtProd_Marca = GetDlgItem(hwnd, TXT_ENVIO_COLONIA);//COLONIA
		hTxtProd_Desc = GetDlgItem(hwnd, TXT_ENVIO_CIUDAD);//CIUDAD
		hTxtProd_Monto = GetDlgItem(hwnd, TXT_ENVIO_ESTADO);//ESTADO
		hTxtProd_Nom = GetDlgItem(hwnd, TXT_ENVIO_MENSAJE);// MENSAJE
		HWND hProdEnv = GetDlgItem(hwnd, CB_PRODUCTOS);

		
		SendMessage(hProdEnv, CB_ADDSTRING, NULL, (LPARAM)aProd->Pnombre.c_str()); //productos mostrados en los envios
	}break;

	case WM_COMMAND: {

		if (LOWORD(wparam) == CB_PRODUCTOS && HIWORD(wparam) == CBN_SELCHANGE) {
			HWND hProdEnv = GetDlgItem(hwnd, CB_PRODUCTOS);
			char cProductos[50];	//almacenamos el texto seleccionado de envios
			int selIndex = SendMessage(hProdEnv, CB_GETCURSEL, NULL, NULL);
			SendMessage(hProdEnv, CB_GETLBTEXT, (WPARAM)selIndex, (LPARAM)cProductos);
			prodtemp.append(cProductos);//el producto elegido

		}
		HWND hDtpTiempo = GetDlgItem(hwnd, DTP_fecha);
		int dtpSize = GetWindowTextLength(hDtpTiempo);
		char cadenaDelDTP[80];
		GetWindowText(hDtpTiempo, cadenaDelDTP, ++dtpSize);//fecha elegida 
		string fechaDTP(cadenaDelDTP);
		switch (LOWORD(wparam)) {
		case BTN_ACEPTAR_ENVIO: {
			string prodnom = getText(hTxtProd_Nom);
			string prodinv = getText(hTxtProd_Inv);
			string prodcod = getText(hTxtProd_Cod);
			string prodmar = getText(hTxtProd_Marca);
			string proddesc = getText(hTxtProd_Desc);
			string prodmon = getText(hTxtProd_Monto);

			if (prodnom.compare("") == 0 || prodinv.compare("") == 0 || prodcod.compare("") == 0 || prodmar.compare("") == 0 || proddesc.compare("") == 0 || prodmon.compare("") == 0 || prodtemp.compare("") == 0) {
				MessageBox(NULL, "Falto llenar la informacion del envio", "No procede el envio", MB_ICONASTERISK);
				break;
			}
			string year = fechaDTP.substr(6, 4);//a;o escogido
			string mon = fechaDTP.substr(3, 2);
			string day = fechaDTP.substr(0, 2);
			string year1 = fecha.substr(6, 4);
			string mon1 = fecha.substr(3, 2);
			string day1 = fecha.substr(0, 2);
			SYSTEMTIME fechaEnvio, fechaActual;
			DateTime_GetSystemtime(hDtpTiempo, &fechaEnvio);
			GetLocalTime(&fechaActual);
			fechaActual.wHour = 0;
			fechaActual.wMilliseconds = 0;
			fechaActual.wMinute = 0;
			fechaActual.wSecond = 0;
			fechaEnvio.wHour = 0;
			fechaEnvio.wMilliseconds = 0;
			fechaEnvio.wMinute = 0;
			fechaEnvio.wSecond = 0;
			double fecha1, fecha2;
			if (!SystemTimeToVariantTime(&fechaEnvio, &fecha1)) { // Convierto la fecha1 a double si falla la fecha es 0
				fecha1 = 0;
			}
			if (!SystemTimeToVariantTime(&fechaActual, &fecha2)) { // Convierto la fecha2 a double si falla la fecha es 0
				fecha2 = 0;
			}
			bool condicionOrden;
			if (fecha1 >= fecha2) {
				
			




			if (oEnvios == NULL) {
				oEnvios = new Envios;
				oEnvios->IDUser = userAccess->IDUser;
				oEnvios->Enprod.append(prodnom);
				oEnvios->cantidad.append(prodinv);
				//oEnvios->monto.append(prodcod);
				oEnvios->calle.append(prodcod);
				oEnvios->colonia.append(prodmar);
				oEnvios->ciudad.append(proddesc);
				oEnvios->estado.append("activo");
				oEnvios->mensaje.append(prodnom);
				oEnvios->fecha.append(cadenaDelDTP);

				oEnvios->next = NULL;
				oEnvios->prev = NULL;
				aEnvios = oEnvios;

			}
			else {
				while (aEnvios->next != NULL)
					aEnvios = aEnvios->next;
				aEnvios->next = new Envios;
				aEnvios->next->prev = aEnvios;
				aEnvios = aEnvios->next;
				aEnvios->IDUser = userAccess->IDUser;
				aEnvios->Enprod.append(prodnom);
				aEnvios->cantidad.append(prodinv);
				//oEnvios->monto.append(prodcod);
				aEnvios->calle.append(prodcod);
				aEnvios->colonia.append(prodmar);
				aEnvios->ciudad.append(proddesc);
				aEnvios->estado.append("activo");
				aEnvios->mensaje.append(prodnom);
				aEnvios->fecha.append(cadenaDelDTP);
				aEnvios->next = NULL;
				aEnvios = oEnvios;

			}
		}
			else {
				MessageBox(NULL, "La fecha1 es menor o igual a la fecha actual", "Selecione otra fecha", MB_ICONASTERISK);
				break;
			}
			saveenvios(aEnvios);
			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgram= false;
			DestroyWindow(hwnd);
		}break;
		 case BTN_CANCELAR_ENVIO: {
			 HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			 hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			 SetMenu(hprincipal, hMenuOpciones);
			 ShowWindow(hprincipal, SW_SHOW);
			 exitProgram = false;
			 DestroyWindow(hwnd);
		 }break;
		}
	}break;
	case WM_TIMER: {
		time(&actualtime);
		timeInfo = localtime(&actualtime);
		char cadenaDeFecha[80];
		strftime(cadenaDeFecha, 80, "%d-%m-%Y                %I:%M:%S", timeInfo);
		HWND hlTiempo = GetDlgItem(hwnd, LBL_Fecha);
		SetWindowText(hlTiempo, cadenaDeFecha);
		 fecha.assign(cadenaDeFecha);
	    
	}break;
	case WM_CLOSE://Evento que se manda a llamar al darle click al boton X
		KillTimer(hwnd, MI_TIMER_GENERADO);
		exitProgram = true;
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY://Evento que se manda a llamar al ejecutar la funcion DestroyWindow
		if (exitProgram)
			PostQuitMessage(9);
		break;
	}
	

	return 0;
}

BOOL CALLBACK fInfoVendedor(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {//ventana donde se muestra la informacion del vendedor 
	switch (msg) {
	case WM_INITDIALOG: {
						if (oInf != NULL) {
							bool found = true;
							while (aInf != NULL) {
								if (aInf->IDUser == userAccess->IDUser) {
									found = true;
									break;
								}
								if (aInf->next != NULL) {
									found = false;
									aInf = aInf->next;
								}
							}
							if (found == true) {

								userInfo = aInf;
								aInf = oInf;
							}
						}
					
						strcpy(cDirFile, userInfo->imagencom.c_str());
						char IdInfo[20];
						_itoa(userInfo->IDUser, IdInfo, 10);
						hTxtInfoNombre = GetDlgItem(hwnd, LBL_Nombre_vendedor);
						hTxtInfoEmpresa = GetDlgItem(hwnd, LBL_Nombre_empresa);
						hTxtInfoId = GetDlgItem(hwnd, LBL_ID_vendedor);
						SetWindowText(hTxtInfoNombre, userInfo->Nombrecom.c_str());
						SetWindowText(hTxtInfoEmpresa, userInfo->Aliascom.c_str());
						SetWindowText(hTxtInfoId, IdInfo);
						HBITMAP hbimagen = (HBITMAP)LoadImage(NULL, cDirFile, IMAGE_BITMAP, 400, 600, LR_LOADFROMFILE);
						HWND hPbImagen = GetDlgItem(hwnd, PB_INFO_IMG);
						SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbimagen);
						
						break;
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_CNL_INFO: {

			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
			break;
		}

		case BTN_CAM_INFV: {
						   HWND hcam = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_CAM_INFO), NULL, fcamInfo);
						   ShowWindow(hcam, SW_SHOW);
						   exitProgram = false;
						   DestroyWindow(hwnd);
						   break;
		}
		}
	}
	//case WM_CLOSE://Evento que se manda a llamar al darle click al boton X
	//	
	//	exitProgram = true;
	//	DestroyWindow(hwnd);
	//	break;
	case WM_DESTROY://Evento que se manda a llamar al ejecutar la funcion DestroyWindow
		if (exitProgram)
			PostQuitMessage(9);
		break;
	}


	return 0;
}
BOOL CALLBACK fcamInfo(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {//ventana donde se cambia la informacion del vendedor 
	
	switch (msg) {
	case WM_INITDIALOG: {
		loadInfo();
		hTxtInfVendedor = GetDlgItem(hwnd, TXT_Cam_inf);
		hTxtInfAlias = GetDlgItem(hwnd, TXT_CAM_alias);
		aInf = userInfo;
		
	
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_Acp_cam_inf: {
			string Name = getText(hTxtInfVendedor);
			string Alias = getText(hTxtInfAlias);
			if (Name.compare("") == 0 || Alias.compare("") == 0) {
				MessageBox(NULL, "Falto llenar la informacion de usuario", " ", MB_ICONASTERISK);
				break;
			}
			if (aInf->next == NULL && aInf->prev == NULL) { //UNICO
				

				MessageBox(NULL, Mostrar, "El Producto Encontrado es:", MB_ICONQUESTION);

				int msgboxID = MessageBox(NULL, "Desea eliminar el producto?", "Eliminar producto", MB_ICONQUESTION | MB_OKCANCEL);
				switch (msgboxID) {
				case IDOK: {
					delete aInf; //Borro el unico elemento

					oInf = aInf = NULL; //Asigno origen y aux a NULL permitiendo que el if del modulo ALTA pueda entrar a la lista vacia

				}break;
				case IDCANCEL:// you check msdn for more cases
				{


				}break;

				}break;

			}
			else if (aProd->next == NULL) { //ULTIMO
				strcpy(Mostrar, aProd->Pnombre.c_str());

				MessageBox(NULL, Mostrar, "El Producto Encontrado es:", MB_ICONQUESTION);
				int msgboxID = MessageBox(NULL, "Desea eliminar el producto?", "Eliminar producto", MB_ICONQUESTION | MB_OKCANCEL);
				switch (msgboxID) {
				case IDOK: {
					aProd->prev->next = NULL; // De aux (ultimo elemento) viaje a prev (Penultimo elemento) accedi a su sig (puntero) lo iguale a NULL
					//Convirtiendo el penultimo elemento en el ultimo
					delete aProd; //Borre aux

					aProd = oProd; //Regrese aux al origen
					saveProd(aProd);
					freeMemoryProd();

				}
				case IDCANCEL:// you check msdn for more cases
				{


				}break;
				}break;
			}
			else if (aProd->prev == NULL) { //PRIMERO
				strcpy(Mostrar, aProd->Pnombre.c_str());

				MessageBox(NULL, Mostrar, "El Producto Encontrado es:", MB_ICONQUESTION);
				int msgboxID = MessageBox(NULL, "Desea eliminar el producto?", "Eliminar producto", MB_ICONQUESTION | MB_OKCANCEL);
				switch (msgboxID) {
				case IDOK: {
					oProd = oProd->next; //Muevo origen el NUEVO INICIO de la lista, como voy a borrar el primero, el segundo elemento se convierte en el nuevo origen
					oProd->prev = NULL; //Al nuevo origen le asigno el previo a NULL para saber que es el inicio de la lista
					delete aProd;//Sin tener ataduras al origen borramos aux;

					aProd = oProd; //Regrese aux al origen
					saveProd(aProd);
					freeMemoryProd();
				}
				case IDCANCEL:// you check msdn for more cases
				{


				}break;
				}
			}
			else { //CUALQUIER OTRO
				strcpy(Mostrar, aProd->Pnombre.c_str());

				MessageBox(NULL, Mostrar, "El Producto Encontrado es:", MB_ICONQUESTION);
				int msgboxID = MessageBox(NULL, "Desea eliminar el producto?", "Eliminar producto", MB_ICONQUESTION | MB_OKCANCEL);
				switch (msgboxID) {
				case IDOK: {
					aProd->prev->next = aProd->next; // De aux(Nodo Actual) obtengo prev (Nodo anterior) de el obtengo el puntero(Sig) y lo asigno a aux->sig(Nodo siguiente)
					aProd->next->prev = aProd->prev;// De aux(Nodo Actual) obtengo sig (Nodo siguiente) de el obtengo el puntero(Prev) y lo asigno a aux->prev(Nodo Anterior)
					delete aProd; //Elimino aux

					aProd = oProd; //Regrese aux al origen
					saveProd(aProd);
					freeMemoryProd();
				}
				case IDCANCEL:// you check msdn for more cases
				{


				}break;
				}break;
			}
			saveInfo(aInf);
			
			aInf = oInf;
			freeMemoryInfo();
			loadInfo();

			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
		}break;
		}
	}
	case WM_DESTROY://Evento que se manda a llamar al ejecutar la funcion DestroyWindow
		if (exitProgram)
			PostQuitMessage(9);
		break;
	}

	return 0;
}
BOOL CALLBACK fProductos(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {//ventana de mis productos
	switch (msg) {
	case WM_INITDIALOG: {
		
		
		 hTxtSelectedNombre = GetDlgItem(hwnd,TXT_PROD_NOMBRE);
		 hTxtSelectedCodigo= GetDlgItem(hwnd, TXT_PROD_COD_SELECTED);
		 hTxtSelectedCantidad = GetDlgItem(hwnd, TXT_PROD_CAN_SELECTED);
		 hTxtSelectedMarca = GetDlgItem(hwnd, TEXT_PROD_MARCA_SELECTED);
		 hTxtSelectedDescripcion = GetDlgItem(hwnd, TXT_PROD_DES_SELECTED);
		 hTxtSelectedMonto = GetDlgItem(hwnd, TXT_PROD_MONTO_SELECTED);
						hLblProductos = GetDlgItem(hwnd, LB_PRODUCTOS);
						int index = 0;
						while (aProd != NULL) {
							SendMessage(hLblProductos, LB_ADDSTRING, NULL, (LPARAM)aProd->Pnombre.c_str());
							SendMessage(hLblProductos, LB_SETITEMDATA, (WPARAM)index, (LPARAM)aProd->IDUser);
							aProd = aProd->next;

						}
						aProd = oProd;
						
					
	}break;

	case WM_COMMAND: {
		
			if (LOWORD(wparam) == LB_PRODUCTOS && HIWORD(wparam) == LBN_SELCHANGE) {
				int index = SendMessage(hLblProductos, LB_GETCURSEL, NULL, NULL);
				int IdSelectedProd = SendMessage(hLblProductos, LB_GETITEMDATA, (WPARAM)index, NULL);
				bool found = true;
				while (aProd != NULL) {

					if (aProd->IDUser == IdSelectedProd)
						break;
					if (aProd->next == NULL) {
						found = false;
						break;
					}
					aProd = aProd->next;
				}
				if (!found)
					aProd = oProd;
				else {
					char cCod[10];
					//char cCant[100];
					//_itoa(aProd->Pcodig, cCod, 100);
					//_itoa(aProd->Pinv, cCant, 100);
					SetWindowText(hTxtSelectedNombre, aProd->Pnombre.c_str());
					SetWindowText(hTxtSelectedCantidad, aProd->Pinv.c_str());
					SetWindowText(hTxtSelectedCodigo, aProd->Pcodig.c_str());
					SetWindowText(hTxtSelectedMarca, aProd->Pmarca.c_str());
					SetWindowText(hTxtSelectedDescripcion, aProd->Pdescr.c_str());
					SetWindowText(hTxtSelectedMonto, aProd->PMonto.c_str());
					aProd = oProd;
				}
			}
			switch (LOWORD(wparam)) {
		case BTN_Volver_Prod:   {
			freeMemoryProd();
			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgram = false;
			
			DestroyWindow(hwnd);
	    }break;
		    }
	}
					 
	case WM_DESTROY:
		
		if (exitProgram)
			PostQuitMessage(9);
		break;
		
	}
	
	return 0;
}
BOOL CALLBACK fcambios(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {//ventana para buscar el producto a cambiar
	switch (msg) {
	case WM_INITDIALOG: {


		hTxtSelectedNombre = GetDlgItem(hwnd, LBL_CAMBIO_NOMBRE);
		hTxtSelectedCantidad = GetDlgItem(hwnd, LBL_CAMBIO_CANT);
		hTxtSelectedCodigo = GetDlgItem(hwnd, LBL_CAMBIO_CODIGO);
		hTxtSelectedMarca = GetDlgItem(hwnd, LBL_CAMBIO_MARCA);
		hTxtSelectedDescripcion = GetDlgItem(hwnd, LBL_CAMBIO_DES);
		hTxtSelectedMonto = GetDlgItem(hwnd, LBL_CAMBIO_MONTO);
		hLblProductos = GetDlgItem(hwnd, LB_PRODUCTOS);
		SetWindowText(hTxtSelectedNombre, cProd->Pnombre.c_str());
		SetWindowText(hTxtSelectedCantidad, cProd->Pinv.c_str());
		SetWindowText(hTxtSelectedCodigo, cProd->Pcodig.c_str());
		SetWindowText(hTxtSelectedMarca, cProd->Pmarca.c_str());
		SetWindowText(hTxtSelectedDescripcion, cProd->Pdescr.c_str());
		SetWindowText(hTxtSelectedMonto, cProd->PMonto.c_str());
		
	


	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_CAMBIOS_VOLVER: {

			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgram = false;
			DestroyWindow(hwnd);
		}break;
		case BTN_CAMBIOS_SEGUIR: {
	          HWND hcambioss = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_CAM_PROD), NULL, fcambioss);
		      
			  ShowWindow(hcambioss, SW_SHOW);
			  exitProgram = false;
			  DestroyWindow(hwnd);
			  break;
		}
		}
	}
	case WM_DESTROY://Evento que se manda a llamar al ejecutar la funcion DestroyWindow
		if (exitProgram)
			PostQuitMessage(9);
		break;
	

	}
	return 0;
}
BOOL CALLBACK fcambioss(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {//ventana donde se realiza el cambio al producto
	switch (msg) {
	case WM_INITDIALOG: {

		hTxtProd_Nom = GetDlgItem(hwnd, TXT_CAM_NOM);
		hTxtProd_Inv = GetDlgItem(hwnd, TXT_CAM_CAN);
		hTxtProd_Cod = GetDlgItem(hwnd, TXT_CAM_COD);
		hTxtProd_Marca = GetDlgItem(hwnd, TXT_CAM_MARC);
		hTxtProd_Desc = GetDlgItem(hwnd, TXT_CAM_DESC);
		hTxtProd_Monto = GetDlgItem(hwnd, TXT_CAM_MONT);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case BTN_CAM_ACP: {
			string prodnom = getText(hTxtProd_Nom);
			string prodinv = getText(hTxtProd_Inv);
			string prodcod = getText(hTxtProd_Cod);
			string prodmar = getText(hTxtProd_Marca);
			string proddesc = getText(hTxtProd_Desc);
			string prodmon = getText(hTxtProd_Monto);

			if (prodnom.compare("") == 0 || prodinv.compare("") == 0 || prodcod.compare("") == 0 || prodmar.compare("") == 0 || proddesc.compare("") == 0 || prodmon.compare("") == 0) {
				MessageBox(NULL, "Falto llenar la informacion de Producto", "No procede la alta", MB_ICONASTERISK);
				break;
			}
			if (oProd != NULL) {
				bool found = true;
				while (aProd->Pnombre.compare(prodnom) != 0) {
					if (aProd->next == NULL) {

						found = false;
						break;
					}
					aProd = aProd->next;
				}
				if (found == true) {
					MessageBox(NULL, "Ya existe un producto con este nombre", "No procede la alta", MB_ICONASTERISK);
					MessageBox(NULL, "Ingrese otro nombre de producto", " ", MB_ICONASTERISK);
					aProd = oProd;
					break;
				}
				else
					aProd = oProd;
			}
			aProd = cProd;
			
				
				
			

			cProd->Pnombre.assign(prodnom);
			cProd->Pinv.assign(prodinv);
			cProd->Pcodig.assign(prodcod);
			cProd->Pmarca.assign(prodmar);
			cProd->Pdescr.assign(proddesc);
			cProd->PMonto.assign(prodmon);
			
			saveProd(cProd);


			aProd = oProd;
			freeMemoryProd();
			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgramRegister = false;
			DestroyWindow(hwnd);
		}break;
		case BTN_CAM_CAN: {
			HWND hprincipal = CreateDialog(hGlobalInst, MAKEINTRESOURCE(DLG_PRIN), NULL, fPrincipal);
			hMenuOpciones = LoadMenu(hGlobalInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hprincipal, hMenuOpciones);
			ShowWindow(hprincipal, SW_SHOW);
			exitProgramRegister = false;
			DestroyWindow(hwnd);
		}

		}
	}break;

	case WM_DESTROY://Evento que se manda a llamar al ejecutar la funcion DestroyWindow
		if (exitProgram)
			PostQuitMessage(9);
		break;
	}
	return FALSE;

	
}
string getText(HWND hwnd) {// funcion que detecta que no haya espacios vacios
	int length = GetWindowTextLength(hwnd);
	if (length < 1)
		return"";
	char cText[50];
	GetWindowText(hwnd, cText, ++length);	
	string sText(cText);
	return sText;
}
void getGlobalid() {
	LectorEscritor.open("GlobalId.txt", ios::in);
	if (LectorEscritor.is_open()) {
		LectorEscritor >> GLOBAL_USER_ID;
		LectorEscritor.close();
		MessageBox(NULL, "CARGA CON EXITO DEL ID!", "CARGA DE ID", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO CARGO ID", MB_ICONASTERISK);
		return;
	}
}
void save(User* pOrigen) {
	LectorEscritor.open("Usuario.bin", ios::out | ios::trunc | ios::binary);
	if (LectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			LectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(User));
			pOrigen = pOrigen->next;
		}
		LectorEscritor.close();
		MessageBox(NULL, "El Guardado  fue exitoso", "GUARDADO", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDADO", MB_ICONASTERISK);
		return;
	}
}
void saveenvios(Envios*pOrigen) {
	LectorEscritor.open("Envios.bin", ios::out | ios::trunc | ios::binary);
	if (LectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			LectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(Envios));
			pOrigen = pOrigen->next;
		}
		LectorEscritor.close();
		MessageBox(NULL, "El Guardado  fue exitoso", "GUARDADO", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo de envios", "NO GUARDADO", MB_ICONASTERISK);
		return;
	}
}
void saveGlobalid() {
	LectorEscritor.open("GlobalId.txt", ios::out | ios::trunc | ios::binary);
	if (LectorEscritor.is_open()) {
		LectorEscritor << GLOBAL_USER_ID;
		LectorEscritor.close();
		
		MessageBox(NULL, "El Guardado  fue exitoso", "GUARDADO", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDADO", MB_ICONASTERISK);
		return;
	}
}
void loadUser() {
	LectorEscritor.open("Usuario.bin", ios::binary | ios::in | ios::ate);
	if (LectorEscritor.is_open()) {
		int totalCharactersUsers = LectorEscritor.tellg();
		if (totalCharactersUsers < 1) {
			MessageBox(NULL, "Archivo Vacio", "NO CARGA", MB_ICONASTERISK);
			return;
		}
		for (int i = 0; i < totalCharactersUsers / sizeof(User); i++) {
			if (oUser == NULL) {
				User* temp = new User;
				oUser = new User;
				LectorEscritor.seekg(i * sizeof(User));
				LectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(User));
				oUser->username = temp->username;
				oUser->IDUser = temp->IDUser;
				oUser->T = temp->T;
				oUser->password = temp->password;
				oUser->next = NULL;
				oUser->prev = NULL;
				aUser = oUser;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			else {
				while (aUser->next != NULL) {
					aUser = aUser->next;
				}
				User* temp = new User;
				aUser->next = new User;
				LectorEscritor.seekg(i * sizeof(User));
				LectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(User));
				aUser->next->prev = aUser;
				aUser = aUser->next;
				aUser->T = temp->T;
				aUser->IDUser = temp->IDUser;
				aUser->username = temp->username;
				aUser->password = temp->password;
				aUser->next = NULL;
				aUser = oUser;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
		}
		LectorEscritor.close();
		MessageBox(NULL, "Carga exitosa", "CARGAR", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO CARGAR", MB_ICONASTERISK);
		return;
	}
}
void saveInfo(Uinfo* pOrigen) {
	LectorEscritor.open("Info.bin", ios::out | ios::trunc | ios::binary);
	if (LectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			LectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(Uinfo));
			pOrigen = pOrigen->next;
		}
		LectorEscritor.close();
		MessageBox(NULL, "El Guardado  fue exitoso", "GUARDADO", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDADO", MB_ICONASTERISK);
		return;
	}
}
void loadInfo() {
	LectorEscritor.open("Info.bin", ios::binary | ios::in | ios::ate);
	if (LectorEscritor.is_open()) {
		int totalCharactersUsers = LectorEscritor.tellg();
		if (totalCharactersUsers < 1) {
			MessageBox(NULL, "Archivo Vacio", "NO CARGA", MB_ICONASTERISK);
			return;
		}
		for (int i = 0; i < totalCharactersUsers / sizeof(Uinfo); i++) {
			if (oInf == NULL) {
				Uinfo* temp = new Uinfo;
				oInf = new Uinfo;
				LectorEscritor.seekg(i * sizeof(Uinfo));
				LectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(Uinfo));
				oInf->Nombrecom = temp->Nombrecom;
				oInf->Aliascom = temp->Aliascom;
				oInf->IDUser = temp->IDUser;
				oInf->imagencom = temp->imagencom;
				oInf->next = NULL;
				oInf->prev = NULL;
				aInf = oInf;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			else {
				while (aInf->next != NULL) {
					aInf = aInf->next;
				}
				Uinfo* temp = new Uinfo;
				aInf->next = new Uinfo;
				LectorEscritor.seekg(i * sizeof(Uinfo));
				LectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(Uinfo));
				aInf->next->prev = aInf;
				aInf = aInf->next;
				aInf->Nombrecom = temp->Nombrecom;
				aInf->Aliascom = temp->Aliascom;
				aInf->IDUser = temp->IDUser;
				aInf->imagencom = temp->imagencom;

				aInf->next = NULL;
				aInf = oInf;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
		}
		LectorEscritor.close();
		MessageBox(NULL, "Carga exitosa", "CARGAR", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO CARGAR", MB_ICONASTERISK);
		return;
	}
}
void saveProd(Productos* pOrigen) {
	LectorEscritor.open("Productos.bin", ios::out | ios::trunc | ios::binary);
	if (LectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			LectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(Productos));
			pOrigen = pOrigen->next;
		}
		LectorEscritor.close();
		MessageBox(NULL, "El Guardado  fue exitoso", "GUARDADO", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDADO", MB_ICONASTERISK);
		return;
	}
}
void loadProd() {
	LectorEscritor.open("Productos.bin", ios::binary | ios::in | ios::ate);
	if (LectorEscritor.is_open()) {
		int totalCharactersUsers = LectorEscritor.tellg();
		if (totalCharactersUsers < 1) {
			remove("Productos.bin");
			MessageBox(NULL, "Archivo Vacio", "NO CARGA", MB_ICONASTERISK);
			return;
		}
		for (int i = 0; i < totalCharactersUsers / sizeof(Productos); i++) {
			if (oProd == NULL) {
				Productos* temp = new Productos;
				oProd = new Productos;
				LectorEscritor.seekg(i * sizeof(Uinfo));
				LectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(Productos));
				oProd->Pnombre = temp->Pnombre;
				oProd->IDUser = temp-> IDUser;
				oProd->Pinv = temp->Pinv;
				oProd->Pcodig = temp->Pcodig;
				oProd->Pmarca = temp->Pmarca;
				oProd->Pdescr = temp->Pdescr;
				oProd->PMonto = temp->PMonto;
				oProd->next = NULL;
				oProd->prev = NULL;
				aProd = oProd;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			else {
				while (aProd->next != NULL) {
					aProd = aProd->next;
				}
				Productos* temp = new Productos;
				aProd->next = new Productos;
				LectorEscritor.seekg(i * sizeof(Productos));
				LectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(Productos));

				aProd->next->prev = aProd;
				aProd = aProd->next;
				aProd->Pnombre = temp->Pnombre;
				aProd->IDUser = temp->IDUser;
				aProd->Pinv = temp->Pinv;
				aProd->Pcodig = temp->Pcodig;
				aProd->Pmarca = temp->Pmarca;
				aProd->Pdescr = temp->Pdescr;
				aProd->PMonto = temp->PMonto;
				aProd->next = NULL;
				aProd = oProd;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
		}
		LectorEscritor.close();
		MessageBox(NULL, "Carga exitosa de productos", "CARGAR", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo de productos", "NO CARGAR", MB_ICONASTERISK);
		
		remove("Productos.bin");
		return;
	}
}
void saveEnvios(Envios* pOrigen) {
	LectorEscritor.open("Envios.bin", ios::out | ios::trunc | ios::binary);
	if (LectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			LectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(Envios));
			pOrigen = pOrigen->next;
		}
		LectorEscritor.close();
		MessageBox(NULL, "El Guardado  fue exitoso", "GUARDADO", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDADO", MB_ICONASTERISK);
		return;
	}
}
void loadEnvios(){
	LectorEscritor.open("Envios.bin", ios::binary | ios::in | ios::ate);
	if (LectorEscritor.is_open()) {
		int totalCharactersUsers = LectorEscritor.tellg();
		if (totalCharactersUsers < 1) {
			
			MessageBox(NULL, "Archivo Vacio", "NO CARGA", MB_ICONASTERISK);
			return;
		}
		for (int i = 0; i < totalCharactersUsers / sizeof(Envios); i++) {
			if (oEnvios == NULL) {
				Envios* temp = new Envios;
				oEnvios = new Envios;
				LectorEscritor.seekg(i * sizeof(Uinfo));
				LectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(Envios));
				oEnvios->Enprod= temp->Enprod;
			
				oEnvios->cantidad = temp->cantidad;
				oEnvios->monto= temp->monto;
				oEnvios->calle = temp->calle;
				oEnvios->colonia= temp->colonia;
				oEnvios->ciudad = temp->ciudad;
				oEnvios->estado = temp->estado;
				oEnvios->mensaje = temp->mensaje;
				oEnvios->fecha = temp->fecha;
				oEnvios->next = NULL;
				oEnvios->prev = NULL;
				aEnvios = oEnvios;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			else {
				while (aEnvios->next != NULL) {
					aEnvios = aEnvios->next;
				}
				Envios* temp = new Envios;
				aEnvios->next = new Envios;
				LectorEscritor.seekg(i * sizeof(Envios));
				LectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(Envios));

				aEnvios->Enprod = temp->Enprod;

				aEnvios->cantidad = temp->cantidad;
				aEnvios->monto = temp->monto;
				aEnvios->calle = temp->calle;
				aEnvios->colonia = temp->colonia;
				aEnvios->ciudad = temp->ciudad;
				aEnvios->estado = temp->estado;
				aEnvios->mensaje = temp->mensaje;
				aEnvios->fecha = temp->fecha;
				aEnvios->next = NULL;
				aEnvios = oEnvios;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
		}
		LectorEscritor.close();
		MessageBox(NULL, "Carga exitosa de productos", "CARGAR", MB_ICONASTERISK);
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo de productos", "NO CARGAR", MB_ICONASTERISK);

		remove("Productos.bin");
		return;
	}
}
void freeMemoryUser() {
	if (oUser != NULL) { //Revisar el origen es null ? NO, liberamos la memoria
		while (aUser != NULL) { //Mientras aux NO sea NULL ? avanzamos
			User* temp = aUser; //Guardamos en un temporal aux
			aUser = aUser->next; //Avanzamos
			delete temp; //Borramos el temporal
		}
	}
	else //Origen si fue null, no hacemos nada
		return;
	aUser = oUser = NULL;
}
void freeMemoryProd() {
	if (oProd != NULL) { //Revisar el origen es null ? NO, liberamos la memoria
		while (aProd != NULL) { //Mientras aux NO sea NULL ? avanzamos
			Productos* temp = aProd; //Guardamos en un temporal aux
			aProd = aProd->next; //Avanzamos
			delete temp; //Borramos el temporal
		}
	}
	else //Origen si fue null, no hacemos nada
		return;
	aProd = oProd = NULL;
}
void freeMemoryInfo() {
	if (oInf != NULL) { //Revisar el origen es null ? NO, liberamos la memoria
		while (aInf != NULL) { //Mientras aux NO sea NULL ? avanzamos
			Uinfo* temp = aInf; //Guardamos en un temporal aux
			aInf = aInf->next; //Avanzamos
			delete temp; //Borramos el temporal
		}
	}
	else //Origen si fue null, no hacemos nada
		return;
	aInf = oInf = NULL;
}

//mostrar envios, el envio que se realice bien y todo lo demas, asignar el id a los envios 