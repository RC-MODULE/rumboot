/*
********************************************************************************
*                               ЗАО НТЦ "Модуль"
*
* Описание    : Тестирование внешней SRAM
*
* Программисты: Манвелян А.А. (12.02.2018)
*               Леонтьев С.Н. (19.06.2018)
*
* Изменения   :
*               Леонтьев С.Н. (13.01.2021)
*                   Портирование теста на ОИ-10
*
********************************************************************************
*/


/*
********************************************************************************
*                               Раздел #include
********************************************************************************
*/

// #include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// #include  <1888TX018/clock.h>
// #include  <1888TX018/gpio.h>
#include <rumboot/printf.h>

#include "crypto.h"


/*
********************************************************************************
*                           Раздел #define constants
********************************************************************************
*/

#define BASE_ADDR       0x100000
#define BASE_ADDR_2     0x300000
// 262144 => (256 * 4) = 1Мбайт - размер тестируемой области для 1-го BASE_ADDR
#define MAX_ADDR        0x040000
#define BUS_ADDR_WIDTH        17    // Бит старшей единички в числе (MAX_ADDR >> 2)
#define NUM_ITER               1    // Количество итераций полного теста

// Количество проходов по типу теста
#define  STEP_NUM_WALK      16
#define  STEP_NUM_INVERS     2
#define  STEP_NUM_RAND       8


/*
********************************************************************************
*                             Локальные переменные
********************************************************************************
*/

static  uint32_t   *s_MemPtr = ( void * )BASE_ADDR;
static  uint32_t    s_Errors = 0;
static  int32_t     s_Result = 0;


/*
********************************************************************************
*              Локальные прототипы функций (в порядке реализации)
********************************************************************************
*/

static  void  checkValueInMem   ( uint32_t status, uint32_t step, uint32_t i_rd );

static  void  initNewTestType   ( uint32_t *p_status, uint32_t *p_step );

// static  void  printStepInfo     ( uint32_t status, uint32_t step, uint64_t time1 );
static  void  printStepInfo     ( uint32_t status, uint32_t step );

static  void  printTestName     ( uint32_t status );

static  void  writeValueToMem   ( uint32_t status, uint32_t step, uint32_t i_wr );


/*
********************************************************************************
*                              Глобальные функции
********************************************************************************
*/

int main ()
{
    // uint64_t    time1 = 0;
    uint32_t    status;
    uint32_t    step;
    uint32_t    i_wr;
    uint32_t    i_rd;

    // Инициализация первого теста на выполнение
    status = 0x00;
    step = STEP_NUM_WALK;
    printTestName( status );
    i_rd = 0;
    i_wr = 0;

    // time1 = get_ticks();

    int32_t count = ( ( MAX_ADDR << 1 ) + 1 ) * ( 16 + 16 + 2 + 8 + 8 ) * 2 * NUM_ITER;

    for ( int32_t i = 0; i < count; i++ )
    {
        // Цикл росписи памяти
        if ( i_wr != MAX_ADDR )
        {
            writeValueToMem( status, step, i_wr );
            i_wr++;
        }

        // Цикл чтения и проверки корректности данных
        else if ( i_rd != MAX_ADDR )
        {
            checkValueInMem( status, step, i_rd );
            i_rd++;
        }

        // Инициализация нового прохода теста
        else
        {
            // printStepInfo( status, step, time1 );
            printStepInfo( status, step );

            step--;

            if ( step == 0 )
            {
                initNewTestType( &status, &step );
                printTestName( status );
            }

            s_Errors = 0;
            i_rd = 0;
            i_wr = 0;

            // time1 = get_ticks();
        }
    }

    rumboot_printf( "Exit test with %ld errors\n", s_Result );
    return s_Result ? 1 : 0;
}


/*
********************************************************************************
*                              Локальные функции
********************************************************************************
*/

static  void  checkValueInMem ( uint32_t status, uint32_t step, uint32_t i_rd )
{
    uint32_t    value;
    uint32_t    y;
    uint32_t    z;
    uint32_t    offset_rd;

    switch ( status & 0x0f )
    {
        case 0:
            offset_rd = i_rd;
            value = 1U << ( ( step + offset_rd ) & 0xf );
            value = value | ( value << 16 );
            break;

        case 1:
            offset_rd = i_rd;
            value = 1U << ( ( step + offset_rd ) & 0xf );
            value = ~( value | ( value << 16 ) );
            break;

        case 2:
            offset_rd = i_rd;
            value = ( step + offset_rd ) & 1 ? 0xa53c5ac3 : 0x5ac3a53c;
            break;

        case 3:
            offset_rd = i_rd;
            value = prand( offset_rd, step, 32 );
            break;

        case 4:
            offset_rd = prand( i_rd, step, BUS_ADDR_WIDTH );
            value = prand( offset_rd, step, 32 );
            break;

        default:
            value = 0xbabadeda;
            offset_rd = 0;
            rumboot_printf( "Internal test error. status = %02lx\n", status );
            exit( 2 );
    }

    y = s_MemPtr[offset_rd];

    if ( y != value )
    {
        z = s_MemPtr[offset_rd];
        s_Result++;
        s_Errors++;

        if ( y != z )
        {
            // time1-=get_ticks();
            rumboot_printf( "Read eror at addr %08lx, write %08lx. read: %08lx and %08lx\n", offset_rd, value, y, z );
            // time1+=get_ticks();
        }
        else
        {
            // time1-=get_ticks();
            rumboot_printf( "Write eror at addr %08lx, write %08lx. read: %08lx\n", offset_rd, value, y );
            // time1+=get_ticks();
        }
    }
}


static  void  initNewTestType   ( uint32_t *p_status, uint32_t *p_step )
{
    switch ( *p_status )
    {
        case 0x00:
            *p_status = 0x01;
            *p_step = STEP_NUM_WALK;
            break;

        case 0x01:
            *p_status = 0x02;
            *p_step = STEP_NUM_INVERS;
            break;

        case 0x02:
            *p_status = 0x03;
            *p_step = STEP_NUM_RAND;
            break;

        case 0x03:
            *p_status = 0x04;
            *p_step = STEP_NUM_RAND;
            break;

        case 0x04:
            *p_status = 0x20;
            *p_step = STEP_NUM_WALK;
            s_MemPtr = ( void * )BASE_ADDR_2;
            break;

        case 0x20:
            *p_status = 0x21;
            *p_step = STEP_NUM_WALK;
            break;

        case 0x21:
            *p_status = 0x22;
            *p_step = STEP_NUM_INVERS;
            break;

        case 0x22:
            *p_status = 0x23;
            *p_step = STEP_NUM_RAND;
            break;

        case 0x23:
            *p_status = 0x24;
            *p_step = STEP_NUM_RAND;
            break;

        case 0x24:
            *p_status = 0x00;
            *p_step = STEP_NUM_WALK;
            s_MemPtr = ( void * )BASE_ADDR;
            break;

        default:
            rumboot_printf( "Internal test error. status = %02lx\n", *p_status );
            exit( 2 );
    }
}


// static  void  printStepInfo ( uint32_t status, uint32_t step, uint64_t time1 )
static  void  printStepInfo ( uint32_t status, uint32_t step )
{
    // uint32_t    executed_time;

    switch ( status & 0x0f )
    {
        case 0x00:
        case 0x01:
            rumboot_printf( "Step %02ld/16. ", STEP_NUM_WALK   + 1 - step );
            break;

        case 0x02:
            rumboot_printf( "Step %02ld/02. ", STEP_NUM_INVERS + 1 - step );
            break;

        case 0x03:
        case 0x04:
            rumboot_printf( "Step %02ld/08. ", STEP_NUM_RAND   + 1 - step );
            break;

        default:
            rumboot_printf( "Internal test error. status = %02lx\n", status );
            exit( 2 );
    }

    // Расчет времени выполнения
    // executed_time = ( uint32_t )( ( get_ticks() - time1 ) / CLOCKS_PER_MSC );

    // rumboot_printf( "Time: %5ld ms. Errors: %2ld \n", executed_time, s_Errors );
    rumboot_printf( "Errors: %2ld \n", s_Errors );
}


static  void  printTestName ( uint32_t status )
{
    switch ( status )
    {
        case 0x00:
            rumboot_printf( "SRAM1. Cache: write-back. Type: walking  one        \n" );
            break;

        case 0x01:
            rumboot_printf( "SRAM1. Cache: write-back. Type: walking zero        \n" );
            break;

        case 0x02:
            rumboot_printf( "SRAM1. Cache: write-back. Type: inverse             \n" );
            break;

        case 0x03:
            rumboot_printf( "SRAM1. Cache: write-back. Type: line addr-rand data \n" );
            break;

        case 0x04:
            rumboot_printf( "SRAM1. Cache: write-back. Type: rand addr-rand data \n" );
            break;

        case 0x20:
            // rumboot_printf( "SRAM1. Cache: none.       Type: walking  one        \n" );
            rumboot_printf( "SRAM2. Cache: write-back. Type: walking  one        \n" );
            break;

        case 0x21:
            // rumboot_printf( "SRAM1. Cache: none.       Type: walking zero        \n" );
            rumboot_printf( "SRAM2. Cache: write-back. Type: walking zero        \n" );
            break;

        case 0x22:
            // rumboot_printf( "SRAM1. Cache: none.       Type: inverse             \n" );
            rumboot_printf( "SRAM2. Cache: write-back. Type: inverse             \n" );
            break;

        case 0x23:
            // rumboot_printf( "SRAM1. Cache: none.       Type: line addr-rand data \n" );
            rumboot_printf( "SRAM2. Cache: write-back. Type: line addr-rand data \n" );
            break;

        case 0x24:
            // rumboot_printf( "SRAM1. Cache: none.       Type: rand addr-rand data \n" );
            rumboot_printf( "SRAM2. Cache: write-back. Type: rand addr-rand data \n" );
            break;

        default:
            rumboot_printf( "Internal test error. status = %02lx\n", status );
            exit( 2 );
    }
}


static  void  writeValueToMem ( uint32_t status, uint32_t step, uint32_t i_wr )
{
    uint32_t    offset_wr;
    uint32_t    value;

    offset_wr = i_wr;

    switch ( status & 0x0f )
    {
        case 0:
            value = 1U << ( ( step + offset_wr ) & 0xf );
            value = value | ( value << 16 );
            break;

        case 1:
            value = 1U << ( ( step + offset_wr ) & 0xf );
            value = ~( value | ( value << 16 ) );
            break;

        case 2:
            value = ( step + offset_wr ) & 1 ? 0xa53c5ac3 : 0x5ac3a53c;
            break;

        case 3:
            value = prand( offset_wr, step, 32 );
            break;

        case 4:
            offset_wr = prand( i_wr, step, BUS_ADDR_WIDTH );
            value = prand( offset_wr, step, 32 );
            break;

        default:
            value = 0;
            rumboot_printf( "Internal test error. status = %02lx\n", status );
            exit( 2 );
    }

    s_MemPtr[offset_wr] = value;
}
