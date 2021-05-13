#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <MMSystem.h>
#include <time.h>
#include "mylib.h"
#define MAX 100
#define D 200 // Mức độ chơi
int x1 = 10,x2 = 100,y1 = 1,y2 = 26; // Tọa độ của tường
int toadox[MAX],toadoy[MAX];
using namespace std;
void Ve_tuong_tren(int x1,int x2,int y1,int y2);
void Ve_tuong_duoi(int x1,int x2,int y1,int y2);
void Ve_tuong_phai(int x1,int x2,int y1,int y2);
void Ve_tuong_trai(int x1,int x2,int y1,int y2);
void Ve_tuong(int x1,int x2,int y1,int y2); // Vẽ tường (gọi lại 4 hàm trên để vẽ 4 góc của tường
void Xu_ly_ran(int toadox[],int toadoy[],int x,int y);
/*Hàm sử dụng mục đích để in rắn ra màn hình,đệ quy lại hàm Ve_ran*/
void Khoi_tao_ran(int toadox[],int toadoy[]);
/*Nạp giữ liệu vào trong 2 hàm toadox và toadoy để khởi tạo cho con rắn
tại vị trí đầu tiên, khi người chơi mới vào*/
void Ve_ran(int toadox[],int toadoy[]);  // Hàm vẽ rắn
void them(int a[],int x);  // Không sử dụng trong game
void xoa(int a[],int vt); //  Không sử dụng trong game
void Xoa_du_lieu_cu(int toadox[],int toadoy[]); // Hàm dùng để xóa con rắn cũ đi để in con rắn mới ra màn hình
bool Kt_ran_cham_tuong(int x0,int y0); // Hàm kiểm tra rắn chạm tường - Game Over
bool Kt_ran_can_duoi(int toadox[],int toadoy[]); // Hàm kiểm tra rắn cắn đuôi - Game Over
void Tao_qua(int &xqua,int &yqua,int toadox[],int toadoy[]); // Hàm tạo 1 quả ngẫu nhiên trong ô, sau khi rắn đã ăn được quả
int Kt_tao_trung(int x,int y,int toadox[],int toadoy[]); // Kiểm tra xem táo có bị radom vào trong thân của rắn hay không
void Xoa_tao(int xqua,int yqua);  // Nếu mà rắn vẫn chưa kịp ăn mồi thì mồi sẽ di chuyển random sang 4 hướng
void In_diem(int score);  // In ra màn hình số con mồi mà người chơi đã ăn được
void In_tich_luy(int tich_luy);  // In ra màn hình điểm tích lũy mà người chơi đạt được
void In_so_buoc(int count_b);  // In ra màn hình số bước mà con mồi đã thoát được rắn
void Ket_thuc(int &m,int count);  // In ra màn hình kết thúc và hỏi người chơi muốn chơi nữa không
void Di_chuyen_qua(int &xqua,int &yqua,int &count_b);
/*Sau khi quả không được rắn ăn, quả sẽ random 1 số trong các số từ 0 dến 3
để di chuyển ứng với sang phải, sang trái, lên trên, xuống dưới.
Các biến xqua, yqua,count_b phải cập nhật lại để có thể thực hiện tiếp
Đệ quy lại hàm In_so_buoc() để in ra màn hình bước mà mồi đã di chuyển*/
void Nocursortype(); // Hàm ẩn con trỏ soạn thảo trong game
bool Kt_qua(int x,int y,int toadox[],int toadoy[]);
/*Kiểm tra xem khi quả di chuyển có bị đè lên thân rắn hoặc chạm tường hay không*/
int sl;
int main()
{
    Nocursortype();
    bool choi = true;  // Tạo biến để kiểm tra xem còn muốn chơi hay không
    while(choi==true){
        sl = 6;
        int count = 0;   //  Đếm số mồi ăn được
        int count_s = 0;  // Tính điểm tích lũy
        int count_b = 0;  // Đếm số bước chạy của con mồi
        srand((time(NULL)));
        int xqua=0,yqua=0;  // 2 tọa độ quả mồi
        //system("color 7f");
        Tao_qua(xqua,yqua,toadox,toadoy);
        Ve_tuong(x1,x2,y1,y2);
        In_diem(count);
        Khoi_tao_ran(toadox,toadoy);
        Ve_ran(toadox,toadoy);
        int check = 2;
        int x  = 50, y = 13; // vi tri dau tien cua ran
        bool gameover = false;
        PlaySound("AWM_4.wav",NULL, SND_SYNC);
        Sleep(100);
        while(gameover==false){
            // thực hiện di chuyển
            Xoa_du_lieu_cu(toadox,toadoy);
            // Ham dieu khien
            if(_kbhit()==true){
                char kitu = _getch();
                if(kitu==-32)
                {
                    kitu = _getch();
                    if(kitu==72&&check!=0) // di len
                    {
                        check = 1;
                    }
                    else if(kitu==80&&check!=1) // di xuong
                    {
                        check = 0;
                    }
                    else if(kitu==77&&check!=3) // qua phai
                    {
                        check = 2;
                    }
                    else if(kitu==75&&check!=2) // qua trai
                    {
                        check = 3;
                    }
                }
            }
            if(check==0){
                y++;// di xuong
            }
            else if(check==1){
                y--;// di len
            }
            else if(check==2){
                // di sang phai
                x++;
            }
            else if(check==3){
                x--;// di sang trai
            }
            Xu_ly_ran(toadox,toadoy,x,y);
            /*======Kiểm tra rắn có chạm tường hay cắn đuôi hay không=======*/
            if(Kt_ran_cham_tuong(x,y)==true) break;
            else if(Kt_ran_can_duoi(toadox,toadoy)==true) break;
            if(x==xqua&&y==yqua)
            {
                count++;
               // printf("\a");
                //PlaySound("AWM_3.wav",0,0);
                Beep(2000,50);
                Tao_qua(xqua,yqua,toadox,toadoy);
                sl++; //làm đuôi rắn dài ra & nhưng code này không cần
                count_s+=(D-count_b);
                count_b = 0;
                In_tich_luy(count_s);
                In_diem(count);
            }
            else{
                Di_chuyen_qua(xqua,yqua,count_b);
                Sleep(50);
                count_b++;
                if(count_b>(D+count_s)) gameover = true;
            }
        }
        int m;
        Ket_thuc(m,count);
        if(m==1){
            choi = true;
            system("cls");
        }
        else choi = false;
    }
    system("cls");
    Ve_tuong(x1,x2,y1,y2);
    SetColor(10);
    gotoXY(40,13);
    cout<<"______SEE YOU AGAIN_____";
    Sleep(1000);
    system("cls");
    return 0;
}
// x1 = 10 , x2 = 100, y1 = 1 , y2 = 26
void Ve_tuong_tren(int x1,int x2,int y1,int y2){
    int x = x1, y = y1;
    while(x<=x2){
        gotoXY(x,y);
        SetColor(2);
        cout<<char(254);
        x++;
    }
}
void Ve_tuong_duoi(int x1,int x2,int y1,int y2){
    int x = x1, y = y2;
    while(x<=x2){
        gotoXY(x,y);
        SetColor(2);
        cout<<char(254);
        x++;
    }
}
void Ve_tuong_phai(int x1,int x2,int y1,int y2){
    int x = x2, y = y1;
    while(y<=y2){
        gotoXY(x,y);
        SetColor(2);
        cout<<char(219);
        y++;
    }
}
void Ve_tuong_trai(int x1,int x2,int y1,int y2){
    int x = x1, y = y1;
    while(y<=y2){
        gotoXY(x,y);
        SetColor(2);
        cout<<char(219);
        y++;
    }
}
void Ve_tuong(int x1,int x2,int y1,int y2)
{
    Ve_tuong_tren(x1,x2,y1,y2);
    Ve_tuong_duoi(x1,x2,y1,y2);
    Ve_tuong_phai(x1,x2,y1,y2);
    Ve_tuong_trai(x1,x2,y1,y2);
}
void Khoi_tao_ran(int toadox[],int toadoy[])
{
    int x = 50, y = 13;
    for(int i=0;i<sl;i++){
        toadox[i] = x;
        toadoy[i] = y;
        x--;
    }
}
void Ve_ran(int toadox[],int toadoy[])
{
    SetColor(7);
    for(int i =0;i<sl;i++){
        gotoXY(toadox[i],toadoy[i]);
        if(i==0)
        {
            cout<<"O";
            SetColor(9);
        }
        else cout<<"o";
    }
}
void Xoa_du_lieu_cu(int toadox[],int toadoy[])
{
    for(int i = 0;i<sl;i++){
        gotoXY(toadox[i],toadoy[i]);
        cout<<" ";
    }
    Nocursortype();
}
void Xu_ly_ran(int toadox[],int toadoy[],int x,int y)
{
    for(int i=sl;i>0;i--){
        toadox[i]=toadox[i-1];
        toadoy[i]=toadoy[i-1];
    }
    toadox[0]=x;
    toadoy[0]=y;
    Ve_ran(toadox,toadoy);
    Sleep(50);
}
bool Kt_ran_cham_tuong(int x0,int y0)
{
    if(y0==1&&(x0>10&&x0<100)) return true; // ran cham tuong tren
    else if(y0==26&&(x0>10&&x0<100)) return true; // ran cham tuong duoi
    else if(x0==10&&(y0>1&&y0<26)) return true;// ran cham tuong phai
    else if(x0==100&&(y0>1&&y0<26)) return true;   // ran cham tuong trai
    return false;
}
bool Kt_ran_can_duoi(int toadox[],int toadoy[])
{
    int x = toadox[0];
    int y = toadoy[0];
    for(int i=1;i<sl;i++){
        if(toadox[i]==x&&toadoy[i]==y) return true;
    }
    return false;
}
void Tao_qua(int &xqua,int &yqua,int toadox[],int toadoy[])
{
    do{
        // 11<= x <= 99
        xqua = rand()%(99-11+1)+10;
        // 2 <=y<=25
        yqua = rand()%(25-2+1)+1;
    }
    while(Kt_tao_trung(xqua,yqua,toadox,toadoy)==0);
    gotoXY(xqua,yqua);
    SetColor(14);
    cout<<"$";
}
int Kt_tao_trung(int x,int y,int toadox[],int toadoy[])
{
    // Kiểm tra quả tạo ra trùng vào người con rắn
    for(int i=0;i<sl;i++){
        if(toadox[i]==x&&toadoy[i]==y) return 0;
    }
    // Kiểm tra quả tạo ra có nằm trên tường hay không.
    if(x==x1||x==x2||y==y1||y==y2) return 0;
    return 1;
}
void Xoa_tao(int xqua,int yqua)
{
    gotoXY(xqua,yqua);
    SetColor(10);
    cout<<" ";
}
void In_diem(int score)
{
    gotoXY(90,27);
    cout<<"              ";
    gotoXY(90,27);
    cout<<"score: "<<score;
}
void In_tich_luy(int tich_luy)
{
    gotoXY(10,27);
    cout<<"                  ";
    gotoXY(10,27);
    cout<<"Diem tich luy: "<<tich_luy;
}
void In_so_buoc(int count_b)
{
    gotoXY(45,27);
    cout<<"                  ";
    gotoXY(45,27);
    cout<<"so buoc: "<<count_b; // In ra số bước đã bị bỏ lỡ
}
void Ket_thuc(int &m,int count)
{
   // PlaySound("",NULL,SND_FILENAME);
   // Beep(5000,200);
    gotoXY(39,2);
    SetColor(15); //15
    cout<<"__________GAME OVER__________";
    gotoXY(38,10);
    cout<<"DIEM CUA BAN DAT DUOC LA: "<<count;
    gotoXY(101,27);
    gotoXY(30,15);
    cout<<"NHAP VAO MAN HINH 1 DE CHOI LAI HOAC 0 DE THOAT: \n";
    PlaySound("AWM_1.wav",NULL, SND_SYNC);
    gotoXY(79,15);
    cin>>m;
}
void Di_chuyen_qua(int &xqua,int &yqua,int &count_b)
{
      int xtam, ytam;
      int f = 4;
      do{
            f--;
            xtam = xqua, ytam = yqua;
            int check_qua = rand()%(4-0)+0;
            if(check_qua==0){
                ytam--;// di xuong
            }
            else if(check_qua==1){
                ytam++;// di len
            }
            else if(check_qua==2){
                // di sang phai
                xtam++;
            }
            else if(check_qua==3){
                xtam--;// di sang trai
            }
            if(f==0){
                xtam = xqua;
                ytam = yqua;
                break;
            }
      }
      while(Kt_qua(xtam,ytam,toadox,toadoy)==0);
        gotoXY(xqua,yqua);
        Xoa_tao(xqua,yqua);
        xqua = xtam; yqua = ytam;
        gotoXY(xqua,yqua);
        SetColor(14);
      //  PlaySound("AWM_5.wav",NULL,SND_SYNC);
        cout<<"$";
        In_so_buoc(count_b);
}
void Nocursortype()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}
bool Kt_qua(int x,int y,int toadox[],int toadoy[])
{
   // Kiểm tra quả trùng người
   for(int i=0;i<sl;i++)
   {
       if(x==toadox[i]&&y==toadoy[i]) return false;
   }
   // Kiểm tra quả chạm tường
   if(x<=x1||x>=x2||y<=y1||y>=y2) return false;
   return true;
}
