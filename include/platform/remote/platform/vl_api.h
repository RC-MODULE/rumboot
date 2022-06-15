#ifndef VL_API_H
#define VL_API_H
#include <stdint.h>
#include <stdbool.h>

#define VL_API_VERSION 0.94

#define VL_OP_READ				1		 
#define VL_OP_WRITE				2 
#define VL_OP_WAIT				3 
#define VL_OP_UPTIME			4 
#define VL_OP_SET_IO_DELAY		5
#define VL_OP_SHUTDOWN			6
#define VL_OP_SOFT_RESET		7
#define VL_OP_HARD_RESET		8
#define VL_OP_GET_TICKS_PER_US	9
#define VL_OP_GET_API_VERSION	0xA
#define VL_OP_TRACE				0xB

#define VL_OP_DISCONNECT		14
#define VL_OP_SHMEM_LIST		15
#define VL_OP_IRQ_LIST			16
#define VL_OP_IRQ_CONFIGURE		17
#define VL_OP_IRQ_STATUS		18
#define VL_OP_IRQ_ACK     		19

#define VL_OP_ACCEPTED			0x20
#define VL_OP_STARTED			0x40
#define VL_OP_FINISHED			0x80
#define VL_OP_TERMINATED		0xFF


#define VL_EVENT_REG_WRITE		0x1
#define VL_EVENT_REG_READ		0x2
#define VL_EVENT_MEM_WRITE		0x4
#define VL_EVENT_MEM_READ		0x8
#define VL_EVENT_IRQ  			0x100
#define VL_EVENT_FULL_BUFF		0x200
#define VL_EVENT_SEND			0x10000
#define VL_EVENT_RECV			0x20000


#define VL_MAX_DATA_LENGTH		256

#define VL_OK					0
#define VL_ERROR 				255
#define VL_SOCKET_ERROR 		-1


/*!
	\defgroup  vlapi Интерфейсные функции VLAPI
 */



/*!
	\breif структура сообщения
 */	
struct vl_message {
	uint64_t addr; 		/// физический адрес
	uint64_t value;		/// значение
	uint8_t  op;		/// идентификатор команды
	uint8_t  data_len;	/// размер дополнительных данных в массиве data
	uint16_t sync;		/// вспомогательное поле синхронизации обмена сообщениями
	union{
		char data [VL_MAX_DATA_LENGTH];		
		int  intdata[VL_MAX_DATA_LENGTH/32];
	};					/// дополнительное поле данных
	
} __attribute__((packed));

/*!
	\breif структура метаданных разделяемой памяти 
 */	
struct vl_shmem{
	uint32_t 	id;				/// числовой идентификатор разделяемой памяти
	char 	 	name[32];		/// строковой идентификатор разделяемой памяти
	char 		ipc_key[128];	/// путь к мапируемому файлу , в формате шаблона <path>XXXXXX
	uint64_t	sys_addr;		/// физический адрес начала памяти на устройстве
	uint64_t	size;			/// размер разделяемой памяти
	void*		ptr; 			/// виртуальный адрес разделяемой памяти в пространстве программы  
								//! Danger  on 32-64 bit arch
} __attribute__((packed));

struct vl_irq{
	int dummy;
};



/*!
	\breif экземпляр соединения  
 */	
struct vl_instance {
	float version;					/// версия API
	int sockfd;						/// дескриптор сокета
	struct vl_shmem* shared_mem_list; /// указатель на список разделяемых памятей. Последней в списке является детерминирующая структура с признаком vm_shmem::size=0 
	struct vl_irq* irq_mem_list;
	uint64_t ticks_per_us;			/// тактовая частота устройства (тактов/мкс)
	void (*error_handler)(int err); /// указатель на обработчик ошибок 
	int verbose;					/// маска признаков для вывода различных событий. см. флаги VL_EVENT_XXXXXX 
};

#ifdef ASCIIDOXY
/*!
	\breif papa asciidoxyclass
	
 */
class asciidoxy {
public:
#endif

#ifdef __cplusplus
extern "C"{
#endif
	
//-------------- low-level communication verilator api --------------------
//\ingroup
/*!
	
	\brief Функция отправки сообщения 
	 
	\param [in] vl Экземпляр соединения
	\param [in] сообщение
	\return результат отправки
	
	\details Базовая служебная функция отправки сообщения 
 */
int 	vl_send(struct vl_instance *vl, const struct vl_message* message);
/*!
	
	\brief Функция приема сообщения 
	 
	\param [in] vl Экземпляр соединения
	\param [in] сообщение
	\return результат приема 
	
	\details Базовая служебная функция приема сообщения 
 */
int 	vl_recv(struct vl_instance *vl, struct vl_message* message);

//-------------- middle-level communication verilator api --------------------
/*!
	
	\brief Функция обмена сообщениями 
	 
	\param [in] vl Экземпляр соединения
	\param [in] send_message сообщение с командой от клиента серверу 
	\param [out] send_message сообщение с результатом исполнения команды от сервера клиенту 
	\return результат отправки
	
	\details  Клиентская служебная функция.  Отправляет команду, получает статут приема и результат исполнения от сервера
 */
int 	vl_transaction(struct vl_instance *vl, const struct vl_message* send_message, struct vl_message* recv_message);

/*!

	\brief Вывод сообщения на экран 
	 
	\param [in] vl Экземпляр соединения
	\param [in] msg сообщение
	\return Нет возвращаемых значений 
	
	\details  служебная отладочная функция 
 */
void 	vl_print_message(const struct vl_message* msg);
//--------------- main level verilator clint-api -------------------------------------


/*!

	\brief Команда запрашивает частоту устройства 
	 
	\param [in] vl Экземпляр соединения
	\param [in] сообщение
	\return Нет возвращаемых значений 
	
	\details  служебная отладочная функция 
 */
uint64_t			vl_get_ticks_per_us(struct vl_instance *vl);

//ingroup vlapi
/*!
	
	\brief Возвращает версию VL_API на сервере. 
	 
	\param [in] vl Экземпляр соединения
	\return Целочисленный идентификатор версии VL_API.  Версии 3.141 соответствует значение 3141
	
	\details Функция служит для проверки совместимости версии API, использованных при сборке приложения и эмулятора.
	\code 
	if (vl_get_api_version(vl)!=VL_API_VERSION)
		printf("WARNING: client-server VL_API versions are different!\n");
	\endcode 
 */
int					vl_get_api_version(struct vl_instance *vl);




//	\ingroup vlapi
/*!
	\brief Создает подключение к эмулятору по указанному порту и адресу 

	\param [in] host сетевой адрес. Адрес сервера по умолчанию  127.0.0.1
	\param [in] port номера порта. Номер порта  сервера по умолчанию 3425
	\return Возвращает экземпляр типа vl_instance. NULL в случае ошибки.

 */
struct 				vl_instance* vl_create(const char *host, uint16_t port); /* 0.0.0.0, localhost, /tmp/my.sock */

/*!
	\ingroup vlapi
	\brief Посылает команду эмулятору завершить текущую сессию и разорвать соединение

	\param [in] vl Экземпляр соединения
	\return Нет возвращаемых значений 

 */
void 				vl_disconnect(struct vl_instance *vl);


/*!
	\ingroup vlapi
	\brief Получает список разделяемых памятей от эмулятора
	 
	\param [in,out] vl Экземпляр соединения
	\return Массив разделяемых памятей типа vl_shmem. Последний элемент массива  является терминирующим. Признаком терминирующего элемента является нулевое значение в поле size
			Возвращаемый указатель на массив памятей также сохраняется в поле vl_instance::shared_mem_list экземпляра  vl.
	
	\details Список выделяется динамически внутри функции. По завершении автоматически удаляется при вызове функции \ref vl_destroy;
 */
struct vl_shmem* 	vl_shmem_list(struct vl_instance *vl);

/*!
	\ingroup vlapi
	\brief Отображает разделяемую память в рабочее пространство программы
	 
	\param [in] shmem структура разделяемой памяти
	\return возвращает указатель на отображенную область памяти 
	

 */
void*				vl_shmem_map(struct vl_shmem* shmem);

/*!
	\ingroup
	\brief Освобождает память ранее отображенную функцией \ref vl_shmem_map
	 
	\param [in] shmem Description for shmem
	\return код ошибки
	

 */
int 				vl_shmem_unmap(struct vl_shmem* shmem);

/*!
	\ingroup vlapi
	\brief Устанавливает минимальную задержку при исполнении команды чтения или записи регистра   
	 
	\param [in] vl Экземпляр соединения
	\param [in] read_min_delay задержка на чтение регистра 
	\param [in] write_min_delay задержка на запись регистра 
	\return Нет возвращаемых значений 

 */
void 				vl_set_io_delay(struct vl_instance *vl, uint32_t read_min_delay, uint32_t write_min_delay);

/*!
	\ingroup vlapi
	\brief Устанавливает задержку между командами опроса регистра 
	 
	\param [in] vl Экземпляр соединения
	\param [in] delay Задержка в тактах между командами чтения по одинаковым адресам. 0 - отключает задержку.
	\return Нет возвращаемых значений 
	
	\details Функция устанавливает задержку на исполнение каждой очередной команды чтения регистра, если предыдущая команда чтения обращалась по тому же адресу.
	Механизм позволяет снизить нагрузку как на периферийную шину при непрерывном опросе состояния регистра, так и на сетевой обмен и таким образом увеличить скорость эмуляции. 
	Данную настройку также можно произвести через параметр запуска эмулятора `--polling-delay`.
 */
void 				vl_set_polling_delay(struct vl_instance *vl, uint32_t delay);


// \defgroup vl_iowrite vl_iowrite
// \ingroup vlapi
	
/*!
	\brief Команда записи регистра 
	 
	\param [in] vl Экземпляр соединения
	\param [in] value значение
	\param [in] base_addr адрес регистра
	\return Нет возвращаемых значений 
	
	\details
 */
void 				vl_iowrite32(struct vl_instance *vl, uint32_t const value, uint64_t const base_addr);

//	\defgroup vl_ioread_ioread
//	\ingroup vlapi
/*!
	\brief Команда чтения регистра 
	 
	\param [in] vl Экземпляр соединения
	\param [in] base_addr адрес регистра
	\return Считанное значение регистра 
	
	\details
 */
uint32_t 			vl_ioread32 (struct vl_instance *vl, uint64_t const base_addr);

/*!
	\ingroup vlapi
	\brief Команда завершения работы эмулятора  
	 
	\param [in] vl Экземпляр соединения
	\return Нет возвращаемых значений
	
	\details Функция отправляет команду завершения текущего сеанса связи и закрытия эмулятора 
 */
void				vl_shutdown (struct vl_instance *vl);

/*!
	\ingroup vlapi
	\brief Команда программного сброса 
	 
	\param [in] vl Экземпляр соединения
	\return Нет возвращаемых значений
	
	\details Функция инициирует последовательность команд по программному сбросу устройства  
 */
void				vl_soft_reset(struct vl_instance *vl);
/*!
	\ingroup vlapi
	\brief Команда аппаратного сброса 
	 
	\param [in] vl Экземпляр соединения
	\return Нет возвращаемых значений
	
	\details Функция иницирует аппаратный сброс - сбрасывается внутреннее состояние и счетчик тактов. 
 */
void				vl_hard_reset(struct vl_instance *vl);

/*!
	\ingroup vlapi
	\brief Удаляет экземпляр соединения
	 
	\param [in] vl экземпляр соединения
	\return Нет возвращаемых значений
	
	\details Закрывает порт сетевого соединения. Размапирует память из списка разделяемых памятей. Удаляет список shared_mem_list. Удаляет сам объект vl.
 */
int 				vl_destroy(struct vl_instance *vl);


/*!
	\ingroup vlapi
	\brief Команда задержки на заданное количество тактов 
	 
	\param [in] vl экземпляр соединения
	\param [in] irq_mask маска прерываний
	\param [in] clocks количество тактов задержки 
	\return 0, либо оставшееся кол-во тактов в случае возникновения прерывания
	
	\details Команда устанавливает задержку на количество тактов clocks в течении которых эмулятор будет свободно работать без обработки новых команд. 
	На каждом такте счетчик cloсks декрементируется на единицу до тех пор, пока не достигнет нуля и функция вернет ноль. Если во время ожидания произойдет прерывание, подходящее под маску irq_mask, 
	то функция немедленно завершится, вернув  оставшееся число тактов счетчика clocks. 
 */
 uint32_t 			vl_wait(struct vl_instance *vl, uint32_t irq_mask, const uint64_t clocks);

/*!
	\ingroup vlapi
	\brief Возвращает текущее время в тактах
	 
	\param [in] vl экземпляр соединения
	\return длительность текущей сессии  в тактах  
	
 */
uint64_t 			vl_get_uptime(struct vl_instance *vl);

/*!
	\ingroup vlapi
	\brief Загружает данные из файла по физическому адресу 
	 
	\param [in] vl экземпляр соединения
	\param [in] file путь к файлу
	\param [in] base_addr физический адрес в карте памяти устройства
	\return прочитанное число байт. В случае ошибки - 0, если физический адрес за границами  доступной памяти, либо прочитанные данные не могут быть размещены в памяти целиком.
	
 */
uint32_t			vl_fread (struct vl_instance *vl, const char* file, const uint64_t base_addr);

/*!
	\ingroup vlapi
	\brief Сохраняет данные из памяти в файл 
	 
	\param [in] vl экземпляр соединения
	\param [in] file путь к файлу
	\param [in] base_addr физический адрес
	\param [in] size размер данных в байтах 
	\return записанное число байт . В случае ошибки - 0, если физический адрес выходит за границы  доступной памяти
	
	\details
 */
uint32_t			vl_fwrite(struct vl_instance *vl, const char* file, const uint64_t base_addr, const uint64_t size);

/*!
	\ingroup vlapi
	\brief преобразует виртуальный адрес в адресном пространстве программы в физический адрес на устройстве
	 
	\param [in] vl экземпляр соединения
	\param [in] addr виртуальный  адрес
	\return физический адрес
	
 */
uint64_t  			vl_virt_to_phys(struct vl_instance *vl,volatile void *addr);

/*!
	\ingroup vlapi
	\brief преобразует физический адрес на устройстве в виртуальный в адресном пространстве программы
	 
	\param [in] vl экземпляр соединения
	\param [in] base_addr физический адрес
	\return виртуальный адрес
	
 */
void*				vl_phys_to_virt(struct vl_instance *vl, uint64_t base_addr);

/*!
	\ingroup vlapi
	\brief устанавливает обработчик ошибок  
	 
	\param [in] vl экземпляр соединения
	\param [in] error_handler указатель на функцию-обработчик 
	\return Нет возвращаемых значений
	

 */
void 				vl_set_error_handler(struct vl_instance *vl, void (*error_handler)(int));

/*!
	\ingroup vlapi
	\brief  отправляет команду эмулятору сохранять трассу в файл по выбранному событию  
	 
	\param [in] vl экземпляр соединения
	\param [in] event_id идентификатор события (`VL_EVENT_XXXX`), при наступлении которого информация о соответствующем состоянии будет сохраняться в файл-трассы   
	\param [in] path  путь к файлу-трассы. Относительный путь отсчитывается от  рабочего каталога из которого был запущен эмулятор    
	\return результат исполнения команды от эмулятора
	
	\details Один общий файл может использоваться для логирования нескольких событий. 
	Передача нуля в параметре `path` отключает сохранение трассы. Доступные флаги `VL_EVENT_XXXX` определены в файле `vl_api.h`  
 */
int 				vl_trace(struct vl_instance *vl, int event_id, const char* path);


struct vl_irq*   	vl_irq_list(struct vl_instance *vl);
int 				vl_irq_enable(struct vl_instance *vl, int irq, uint32_t flags);
int 				vl_irq_disable(struct vl_instance *vl, int irq);
int 				vl_irq_get_pending(struct vl_instance *vl);
int 				vl_irq_ack(struct vl_instance *vl, int irq);


 
/*!
	\ingroup vlapi
	\brief Создает разделяемую память 
	 
	\param [in] shmem мета-структура разделяемой памяти
	\return Виртуальный адрес на начало памяти в адресном пространстве программы
	
	\details Функция сервера 
 */
void*				vl_shmem_create(struct vl_shmem* shmem);


#ifdef ASCIIDOXY
};
#endif





//-------------------------------------------------------




void vl_set_async_irq_callback(struct vl_instance *vl, 
	void (*vl_async_irq_handler)(struct vl_instance *vl, int irq, void *arg), 
	void *arg);

//-------------- test api --------------------
//static inline void 		iowrite32(uint32_t const value, uint32_t const base_addr);
//static inline uint32_t 	ioread32(uint32_t const base_addr);
//uint32_t rumboot_platform_request_file_ex(const char *plusarg, uint32_t addr, uint32_t bufsize);
//void 	 rumboot_platform_request_file(const char *plusarg, uint32_t addr);
//int64_t  rumboot_virt_to_phys(volatile void *addr);


void 				vl_iowrite64(struct vl_instance *vl, uint64_t const value, uint64_t const base_addr);
void 				vl_iowrite16(struct vl_instance *vl, uint16_t const value, uint64_t const base_addr);
void 				vl_iowrite8 (struct vl_instance *vl, uint8_t  const value, uint64_t const base_addr);


uint64_t 			vl_ioread64 (struct vl_instance *vl, uint64_t const base_addr);
uint16_t 			vl_ioread16 (struct vl_instance *vl, uint64_t const base_addr);
uint8_t 			vl_ioread8  (struct vl_instance *vl, uint64_t const base_addr);

#ifdef __cplusplus
};
#endif

#endif // !VL_API_H
