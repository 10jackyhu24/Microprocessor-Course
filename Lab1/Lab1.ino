#define NUM 15 // 首先定義 LED 的數目
int leds[NUM] = {
    D0, D1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, D14, D15};
// 將每個腳位模式設為 OUTPUT
void setup()
{
    for (int i = 0; i < NUM; i++)
    {
        pinMode(leds[i], OUTPUT);
    }
}
// 雖然 A3、A2、A1、A0 是類比腳位，
// 但也可以把它當做數位腳位使用。
// 然後是兩個迴圈，
// 第一個迴圈從這邊跑到那邊，
// 第二個迴圈跑回來。
// 參數為 1: 全亮; 參數為 0: 全暗
void AllLight(unsigned int light)
{
    unsigned val = 0xFFFF;
    for (int i = 0; i < NUM; ++i)
    {
        if (val & 0x1)
            digitalWrite(leds[i], light ? LOW : HIGH);
        val >>= 1;
    }
}
// 根據 16 個 0 或 1，顯示對應燈號的亮或暗
void lightCtrl(unsigned light)
{
    unsigned val = light & 0xFFFF;
    for (int i = 0; i < NUM; ++i)
    {
        if (i == 3)
            val >>= 1;
        digitalWrite(leds[i], (val & 0x1) ? LOW : HIGH);
        val >>= 1;
    }
}
// 從中間亮然後熄滅，重複執行到最兩端
void DisplayLEDs(unsigned int dur)
{
    AllLight(0);
    unsigned left = 0x100, right = 0x80;
    unsigned light = left | right;
    for (int i = 0; i < 8; ++i)
    {
        lightCtrl(light);
        delay(dur);
        AllLight(0);
        left <<= 1;
        right >>= 1;
        light = left | right;
    }
}
// 從外面亮然後熄滅，重複執行到中間
void DisplayLEDs2(unsigned int dur)
{
    AllLight(0);
    unsigned left = 0x8000, right = 0x1;
    unsigned light = left | right;
    for (int i = 0; i < 8; ++i)
    {
        lightCtrl(light);
        delay(dur);
        AllLight(0);
        left >>= 1;
        right <<= 1;
        light = left | right;
    }
}
// 全部燈號設為 LOW，從右邊到左邊依序設為 HIGH
void DisplayLEDs3(unsigned int dur)
{
    AllLight(1);
    unsigned val = 0xFFFF;
    for (int i = 0; i < 16; ++i)
    {
        lightCtrl(val);
        val >>= 1;
        delay(dur);
    }
}
// 全部燈號設為 HIGH，從右上角依序設為 LOW
void DisplayLEDs4(unsigned int dur)
{
    AllLight(0);
    unsigned val = 0x1;
    for (int i = 0; i < 16; ++i)
    {
        lightCtrl(val);
        val <<= 1;
        val |= 0x1;
        delay(dur);
    }
}
// 全部燈號設為 HIGH，從右邊依序閃爍兩次
void DisplayLEDs5(unsigned int dur)
{
    AllLight(0);
    unsigned val = 0x1;
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            lightCtrl(val);
            delay(dur);
            AllLight(0);
            delay(dur);
        }
        val <<= 1;
    }
}
void loop()
{
    for (int i = 0; i < 2; ++i)
    {
        DisplayLEDs(300);
    }
    for (int i = 0; i < 2; ++i)
    {
        DisplayLEDs2(300);
    }
    for (int i = 0; i < 2; ++i)
    {
        DisplayLEDs3(300);
    }
    for (int i = 0; i < 2; ++i)
    {
        DisplayLEDs4(300);
    }
    for (int i = 0; i < 2; ++i)
    {
        DisplayLEDs5(150);
    }
}