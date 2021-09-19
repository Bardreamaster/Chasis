# Artinx战队电控代码规范V1.1

  为了减少由于各个队员各人编程风格不同导致的而浪费在尝试相互理解上的时间，有必要统一编程风格。

  为了提高代码的复用性，鼓励使用C++编写高层逻辑代码。
  
  鉴于稳定性、底层debug方便程度、效率综合考虑，鼓励使用意法半导体官方提供的标准固件库做硬件驱动。
  
  由于官方标准库已经不再更新，有些型号的STM32芯片不得不使用HAL库或是LL库，鼓励在项目开始时就确定使用库的版本，**尽可能不改变库的版本**，记住，**最新的不一定是最好的**，对意法半导体官方给的代码尤其如此。

  若使用STM32CubeMX自动生成代码，尽可能减少重新生成代码的次数。
  
  避免写**中文注释**，可以用markdown、LaTex乃至Word写**中文文档**，也不要写**中文注释**，血的教训。

  使用C++是为了方便**并行开发**和**代码复用**，但是滥用C++各种五花八门的特性不仅会导致代码生涩难懂，更加有可能浪费嵌入式芯片宝贵的算力。不推荐在嵌入式上使用以下特性:
   1. 任何C++STL的特性，包括iostream或是vector
   2. 多重继承(超级邪恶)
   3. 动态内存分配，包括new、delete、malloc、free等，真有需要考虑使用内存池

  [ARMCC编译器特性支持程度](http://www.keil.com/support/man/docs/armcc/armcc_chr1407404265784.htm)

 为方便队友理解我们的设计思路，推荐学习[如何看懂UML图](https://design-patterns.readthedocs.io/zh_CN/latest/read_uml.html)并尝试使用UML图解释自己代码的逻辑。上面链接里的文章后面部分是将设计模式的，建议队员有一定项目经验之后再去学习。

 本规范主要参考[Google开源项目C++风格指南](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents/)、《游戏引擎架构》、《游戏人工智能编程案例精粹》，并且在最后用一整个例子解释所有必须遵守的部分，推荐阅读文字说明的同时配上最后的样例代码一起食用。有兴趣的队员可以去原网站阅读。

## 1. 头文件

一个头文件由以下项目按顺序组成:
   
  1. 头文件需要#define保护
  2. 纯C语言需要extern "C"的链接指示
  3. <kbd>.cpp</kbd>文件包含头文件次序如下，<kbd>.h</kbd>文件include顺序类似，需要去掉第一项(就是这个<kbd>.h</kbd>文件自身):
     1. 这个<kbd>.cpp</kbd>文件要实现或是测试的<kbd>.h</kbd>文件
     2. 空行
     3. C 系统文件
     4. 空行
     5. C++ 系统文件
     6. 空行
     7. 其他库的<kbd>.h</kbd>文件
     8. 空行
     9. 本项目的<kbd>.h</kbd>文件

     两行#include之间最多一个空行
  4. 各式#define
  5. 枚举量
  6. 常量
  7. 类定义(C++)或是函数定义(C)，其中，类定义次序如下:
       1.  private变量
       2.  private static变量
       3.  private函数
       4.  private static函数
       5.  protected变量
       6.  protected static变量
       7.  protected函数
       8.  protected static函数
       9.  public变量 (!!! PURE EVIL ! DON'T DO THIS !!!)
       10. public static变量 (!!! PURE EVIL ! DON'T DO THIS !!!)
       11. public函数
       12. public static函数
  8. #endif 后面用注释说明这个#endif对应了哪个#ifdef/#ifndef

## 2. 命名

通用规则: 函数命名, 变量命名, 文件命名要有描述性; 少用缩写

  1. 文件命名
       1. 全部小写
       2. 单词之间用下划线
       3. C++文件以<kbd>.cpp</kbd>结尾，C文件以<kbd>.c</kbd>结尾，头文件以<kbd>.h</kbd>结尾
     
     可以使用的文件名模板(头文件应与cpp同名):
       1. main.cpp
       2. <父类>\_<描述>.cpp 例如 state\_blaster\_overheat.cpp, controller\_chassis.cpp
       3. <类别>\_<描述>.c/cpp 例如 math_pid.cpp, bsp_led.c

  2. 类型命名
       1. 每个单词首字母大写
       2. 全大写的缩写当一个单词看，例如 CAN = Can, ID = Id, DNS = Dns
       3. 不包含下划线
   
     可以使用的类型模板:
       1. <描述><父类> 例如 GimbalController, BlasterOverheatState
       2. <接口类> 例如 Controller, State
       3. <描述>Type, 用于枚举, 例如 CanMotorIdType, MusicNoteType

  3. 变量命名
       1. 全部小写
       2. 单词之间用下划线
       3. 指针命名p_开头
       4. 常量命名k_开头
       5. 全局变量命名g_开头
       6. 优先级 p > k > g, 例如 pkg_xxx, pg_xxx, kg_xxx

  4. 类成员命名
       1. m_开头
       2. 如果是指针，m_p开头
       3. 剩下部分大驼峰
       4. 取值函数格式: Get<成员名>, 比如
       5. 设值函数格式: Set<成员名>, 比如
            
              void SetCurrentSheet(uint32_t m_CurrentSheet);

  5. 函数命名
       1. 全部小写
       2. 单词之间用下划线

  6. 类成员函数命名
       1. 大驼峰

  7. 宏命名
       1. 全部大写
       2. 单词之间用下划线

## 3. 注释

  1. **！！！！不能有中文 ！！！！**
  2. 越少越好，概念性的东西、代码构架等请写在文档里，写文档的时候请参考*Artinx电控文档规范*写
  3. 用<kbd>//</kbd>或是<kbd>/\*  \*/</kbd>都可以，**两者可以同时使用**
  4. <kbd>.h</kbd>文件头模板:
             
         /**
           ****************************************************************
           * @file    controller_buzzer.h
           * @author  TofuLemon(5thzhengjiarandom@gmail.com)
           * @date    2020-Jan-09
           ****************************************************************
           */
        
         #ifndef XXX
         
     @date为最后更新日期, 有闲心可以加上@version, @brief什么的，不过至少要这三项，邮箱不要留学校邮箱

  5. 剩下的参考[这里](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/comments/)

## 4. 样例: 

  
bsp_buzzer.h
```C
/**
  ****************************************************************
  * @file    bsp_buzzer.h
  * @author  TofuLemon(5thzhengjiarandom@gmail.com)
  * @date    2020-Jan-09
  ****************************************************************
  */

#ifndef BSP_BUZZER_H_
#define BSP_BUZZER_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PLAY_ON_RESET 1

#include <string.h>
#include <stdint.h>

#include "stm32f4xx.h"
#include "third_party/id_software/id_Q_rsqrt.h"

#include "misc/misc_music_sheet_1.h"
#include "misc/misc_music_sheet_2.h"

typedef enum
{
    gs3 = 349,
    a_3 = 370,
    as3 = 392,
    b_3 = 415,
    c_4 = 440,
    cs4 = 466,
    d_4 = 523,
    ds4 = 587,
    e_4 = 622,
    f_4 = 659,
    fs4 = 698,
    g_4 = 740,
        
    slt = 0,
    eos = -1,
}MusicNoteType;

void bsp_buzzer_init(void);
void bsp_buzzer_on(void);
void bsp_buzzer_off(void);
void bsp_buzzer_set_freq(uint32_t freq);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BSP_BUZZER_H_ */
```

  C++头文件样例如下:

controller_buzzer.h
```C++
/**
  ****************************************************************
  * @file    controller_buzzer.h
  * @author  TofuLemon(5thzhengjiarandom@gmail.com)
  * @date    2020-Jan-09
  ****************************************************************
  */

#ifndef CONTROLLER_BUZZER_H_
#define CONTROLLER_BUZZER_H_

#include "bsp_buzzer.h"

#include "rtos.h"

#include "controller/controller_base.h"

class BuzzerController : Controller
{
public:
    BuzzerController(void);
    void Init(void);
    void Update(void);
    void On(void);
    void Off(void);

    void SetFreq(uint32_t freq);
    uint32_t GetFreq(void);

    void SetCurrentSheet(uint32_t currentSheet);
    uint32_t GetCurrentSheet(void);

    static uint32_t GetTimeLooped(void);

private:
    uint32_t m_Freq;
    uint32_t m_CurrentSheet;

    static uint32_t m_TimeLooped;
}

#endif /* CONTROLLER_BUZZER_H_ */
```
