#define FINAL
#define TARGET_RPI
#define TYPE_LOWLEVEL
#define LIB_HUB
#define LIB_HID
#define LIB_KBD
#define LIB_MOUSE

typedef enum {
	OK = 0,
	ErrorGeneral = -1,
	ErrorArgument = -2,
	ErrorRetry = -3,
	ErrorDevice = -4,
	ErrorIncompatible = -5,
	ErrorCompiler = -6,
	ErrorMemory = -7,
	ErrorTimeout = -8,
	ErrorDisconnected = -9,
} Result;


/******************************************************************************
*	configuration.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	configuration.h contains definitions for all optional components
*	The makefile defines three main categories of definitions:
*		CONFIG: Whether or not this is a DEBUG driver
*		TARGET: The target system
*		TYPE:	What sort of driver to compile (e.g. standalone)
*****************************************************************************/

// Check we have a CONFIG. Valid choices are DEBUG and FINAL. If neither of
// of these are specified, CONFIG_ERROR will be. If not, the haven't used the
// makefile.
#if defined DEBUG
#elif defined FINAL
#elif defined CONFIG_ERROR
#	error Please specify the CONFIG as either DEBUG or FINAL (default)
#else
#	error Please ensure you compile the driver with the makefile provided
#endif

// Check we have a target. This should either be RPI or NONE. If neither of
// these is specified, TARGET_ERROR will be. If not, the haven't used the
// makefile.
#if defined TARGET_RPI
	// Compiling for the Raspberry Pi (model B). 
	// This is an ARM1176JZF-S, running ARMv6.
	// The chip is a Broadcom 2835 with a Designware OTG Core, mapped to 
	// physical address 0x20980000
#	define ARM
#	define ARM_V6
#	define ENDIAN_LITTLE
#	define BROADCOM_2835
#	define HCD_DESIGNWARE_20
#	define HCD_DESIGNWARE_BASE ((void*)0x20980000)
#elif defined TARGET_NONE
	// Compiling for no target architecture. This will rapidly run into errors.
#elif defined TARGET_ERROR
#	error Please specify the TARGET as either RPI or NONE (default)
#else
#	error Please ensure you compile the driver with the makefile provided
#endif


#if defined TYPE_STANDALONE
	// Disables all logging
#	define NO_LOG
	// Disables external memory management
#	define MEM_INTERNAL_MANAGER
	// Disables external memory reservation
#	define MEM_NO_RESERVE
#elif defined TYPE_LOWLEVEL
	// Disables external memory management
#	define MEM_INTERNAL_MANAGER
#	define MEM_NO_RESERVE
#elif defined TYPE_DRIVER
#elif defined TYPE_ERROR
#	error Please specify the TYPE as either STANDALONE, LOWLEVEL (default) or DRIVER
#else
#	error Please ensure you compile the driver with the makefile provided
#endif


/******************************************************************************
*	types.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	types.h contains definitions of standardised types used ubiquitously.
******************************************************************************/

#ifndef _TYPES_H
#define _TYPES_H
#ifdef __cplusplus
extern "C"
{
#endif

/** Unsigned 8 bit type */
typedef unsigned	char		u8;
/** Unsigned 16 bit type */
typedef unsigned	short		u16;
/** Unsigned 32 bit type */
typedef unsigned	int			u32;
/** Unsigned 64 bit type */
typedef unsigned	long long	u64;
/** Signed 8 bit type */
typedef signed		char		s8;
/** Signed 16 bit type */
typedef signed		short		s16;
/** Signed 32 bit type */
typedef signed		int			s32;
/** Signed 64 bit type */
typedef signed		long long	s64;

/** One bit truth value */
#ifndef __cplusplus
typedef enum {
	false = 0,
	true = 1,
} bool;
#endif

/**
	\brief Result of a method call.

	Negative results are errors.
	OK is for a general success.
	ErrorGeneral is an undisclosed failure.
	ErrorArgument is a bad input.
	ErrorRetry is a temporary issue that may disappear, the method should be rerun
		without modification (the caller is expected to limit number of retries as
		required).
	ErrorDevice is a more permenant hardware error (a reset procedure should be
		enacted before retrying).
	ErrorIncompatible is a device driver that will not support the detected
		device.
	ErrorCompiler is a problem with the configuration of the compiler generating
		unusable code.
	ErrorMemory is used when the memory is exhausted.
	ErrorTimeout is used when a maximum delay is reached when waiting and an 
		operation is unfinished. This does not necessarily mean the operationg 
		will not finish, just that it is unreasonably slow.
	ErrorDisconnected is used when a device is disconnected in transfer.
*/
/**
	\brief Direction of USB communication.

	Many and various parts of the USB standard use this 1 bit field to indicate
	in which direction information flows.
*/
typedef enum {
	HostToDevice = 0,
	Out = 0,
	DeviceToHost = 1,
	In = 1,
} UsbDirection;

/**
	\brief Speed of USB communication.

	Many and various parts of the USB standard use this 2 bit field to indicate
	in which direction information flows.
*/
typedef enum {
	High = 0,
	Full = 1,
	Low = 2,
} UsbSpeed;

static inline char* SpeedToChar(UsbSpeed speed) {
	if (speed == High) return "480 Mb/s";
	else if (speed == Low) return "1.5 Mb/s";
	else if (speed == Full) return "12 Mb/s";
	else return "Unknown Mb/s";
}

/**
	\brief Transfer type in USB communication.

	Many and various parts of the USB standard use this 2 bit field to indicate
	in what type of transaction to use.
*/
typedef enum {
	Control = 0,
	Isochronous = 1,
	Bulk = 2,
	Interrupt = 3,
} UsbTransfer;

/**
	\brief Transfer size in USB communication.

	Many and various parts of the USB standard use this 2 bit field to indicate
	in what size of transaction to use.
*/
typedef enum {
	Bits8,
	Bits16,
	Bits32,
	Bits64,
} UsbPacketSize;

static inline UsbPacketSize SizeFromNumber(u32 size) {
	if (size <= 8) return Bits8;
	else if (size <= 16) return Bits16;
	else if (size <= 32) return Bits32;
	else return Bits64;
}
static inline u32 SizeToNumber(UsbPacketSize size) {
	if (size == Bits8) return 8;
	else if (size == Bits16) return 16;
	else if (size == Bits32) return 32;
	else return 64;
}

#ifdef __cplusplus
}
#endif

/**
	\brief Returns the minimum of two inputs.

	Returns the minimum of two inputs, ensuring only one evaluation of each. To
	do so, the type must be supplied.
*/
#define Min(x, y, type) ({ type __x = (x); type __y = (y); __x < __y ? __x : __y; })
/**
	\brief Returns the maximum of two inputs.

	Returns the maximum of two inputs, ensuring only one evaluation of each. To
	do so, the type must be supplied.
*/
#define Max(x, y, type) ({ type __x = (x); type __y = (y); __x < __y ? __y : __x; })

#endif // _TYPES_H

/******************************************************************************
*	usbd/descriptors.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	usbd/descriptors.h contains structures defined in the USB standard that
*	describe various aspects of USB.
******************************************************************************/
#ifndef _USBD_DESCRIPTORS_H
#define _USBD_DESCRIPTORS_H

#ifdef __cplusplus
extern "C"
{
#endif


/**
	\brief The descriptor type field from the header of USB descriptors.

	The descriptor type in the header of all USB descriptor sturctures defined
	in the USB 2.0 manual in 9.6.
*/
enum DescriptorType {
	Device = 1,
	Configuration = 2,
	String = 3,
	Interface = 4,
	Endpoint = 5,
	DeviceQualifier = 6,
	OtherSpeedConfiguration = 7,
	InterfacePower = 8,
	Hid = 33,
	HidReport = 34,
	HidPhysical = 35,	
	Hub = 41,
};

/**
	\brief The header of USB descriptor information.

	The header of all USB descriptor sturctures defined in the USB 2.0 manual 
	in 9.6.
*/
struct UsbDescriptorHeader {
	u8 DescriptorLength; // +0x0
	enum DescriptorType DescriptorType : 8; // +0x1
} __attribute__ ((__packed__));

/**
	\brief The device descriptor information.

	The device descriptor sturcture defined in the USB 2.0 manual in 9.6.1.
*/
enum DeviceClass {
  DeviceClassInInterface = 0x00,
  DeviceClassCommunications = 0x2,
  DeviceClassHub = 0x9,
  DeviceClassDiagnostic = 0xdc,
  DeviceClassMiscellaneous = 0xef,
  DeviceClassVendorSpecific = 0xff,
};
struct UsbDeviceDescriptor {
	u8 DescriptorLength; // +0x0
	enum DescriptorType DescriptorType : 8; // +0x1
	u16 UsbVersion; // (in BCD 0x210 = USB2.10) +0x2
	enum DeviceClass Class : 8; // +0x4
	u8 SubClass; // +0x5
	u8 Protocol; // +0x6
	u8 MaxPacketSize0; // +0x7
	u16 VendorId; // +0x8
	u16 ProductId; // +0xa
	u16 Version; // +0xc
	u8 Manufacturer; // +0xe
	u8 Product; // +0xf
	u8 SerialNumber; // +0x10
	u8 ConfigurationCount; // +0x11
} __attribute__ ((__packed__));

/**
	\brief The device qualifier descriptor information.

	The device descriptor qualifier sturcture defined in the USB 2.0 manual in 
	9.6.2.
*/
struct UsbDeviceQualifierDescriptor {
	u8 DescriptorLength; // +0x0
	enum DescriptorType DescriptorType : 8; // +0x1
	u16 UsbVersion; // (in BCD 0x210 = USB2.10) +0x2
	enum DeviceClass Class : 8; // +0x4
	u8 SubClass; // +0x5
	u8 Protocol; // +0x6
	u8 MaxPacketSize0; // +0x7
	u8 ConfigurationCount; // +0x8
	u8 _reserved9; // +0x9
} __attribute__ ((__packed__));

/**
	\brief The configuration descriptor information.

	The configuration descriptor structure defined in the USB2.0 manual section
	9.6.3.
*/
struct UsbConfigurationDescriptor {
	u8 DescriptorLength; // +0x0
	enum DescriptorType DescriptorType : 8; // +0x1
	u16 TotalLength; // +0x2
	u8 InterfaceCount; // +0x4
	u8 ConfigurationValue; // +0x5
	u8 StringIndex; // +0x6
	struct {
		unsigned _reserved0_4 : 5; // @0
		bool RemoteWakeup : 1; // @5
		bool SelfPowered : 1; // @6
		unsigned _reserved7 : 1; // @7
	} __attribute__ ((__packed__)) Attributes; // +0x7
	u8 MaximumPower; // +0x8
} __attribute__ ((__packed__));

/**
	\breif The other speed configuration descriptor.

	The other speed configuration descriptor defined in the USB2.0 manual section 
	9.6.4.
*/
struct UsbOtherSpeedConfigurationDescriptor {
	u8 DescriptorLength; // +0x0
	enum DescriptorType DescriptorType : 8; // +0x1
	u16 TotalLength; // +0x2
	u8 InterfaceCount; // +0x4
	u8 ConfigurationValue; // +0x5
	u8 StringIndex; // +0x6
	struct {
		unsigned _reserved0_4 : 5; // @0
		bool RemoteWakeup : 1; // @5
		bool SelfPowered : 1; // @6
		enum {
			Valid = 1,
		} _reserved7 : 1; // @7
	} __attribute__ ((__packed__)) Attributes; // +0x7
	u8 MaximumPower; // +0x8
} __attribute__ ((__packed__));

/**
	\brief The interface descriptor information.

	The interface descriptor structure defined in the USB2.0 manual section 
	9.6.5.
*/
struct UsbInterfaceDescriptor {
	u8 DescriptorLength; // +0x0
	enum DescriptorType DescriptorType : 8; // +0x1
	u8 Number; // +0x2
	u8 AlternateSetting; // +0x3
	u8 EndpointCount; // +0x4
	enum InterfaceClass {
		InterfaceClassReserved = 0x0,
		InterfaceClassAudio = 0x1,
		InterfaceClassCommunications = 0x2,
		InterfaceClassHid = 0x3,
		InterfaceClassPhysical = 0x5,
		InterfaceClassImage = 0x6,
		InterfaceClassPrinter = 0x7,
		InterfaceClassMassStorage = 0x8,
		InterfaceClassHub = 0x9,
		InterfaceClassCdcData = 0xa,
		InterfaceClassSmartCard = 0xb,
		InterfaceClassContentSecurity = 0xd,
		InterfaceClassVideo = 0xe,
		InterfaceClassPersonalHealthcare = 0xf,
		InterfaceClassAudioVideo = 0x10,
		InterfaceClassDiagnosticDevice = 0xdc,
		InterfaceClassWirelessController = 0xe0,
		InterfaceClassMiscellaneous = 0xef,
		InterfaceClassApplicationSpecific = 0xfe,
		InterfaceClassVendorSpecific = 0xff,
	} Class : 8; // +x05
	u8 SubClass;
	u8 Protocol;
	u8 StringIndex;
} __attribute__ ((__packed__));

/**
	\brief The endpoint descriptor information.

	The endpoint descriptor structure defined in the USB2.0 manual section 
	9.6.6.
*/
struct UsbEndpointDescriptor {
	u8 DescriptorLength; // +0x0
	enum DescriptorType DescriptorType : 8; // +0x1
	struct {
		unsigned Number : 4; // @0
		unsigned _reserved4_6 : 3; // @4
		UsbDirection Direction : 1; // @7
	} __attribute__ ((__packed__)) EndpointAddress; // +0x2
	struct {
		UsbTransfer Type : 2; // @0
		enum {
			NoSynchronisation = 0,
			Asynchronous = 1,
			Adaptive = 2,
			Synchrouns = 3,
		} Synchronisation : 2; // @2
		enum {
			Data = 0,
			Feeback = 1,
			ImplicitFeebackData = 2,
		} Usage : 2; // @4
		unsigned _reserved6_7 : 2; // @6
	} __attribute__ ((__packed__)) Attributes; // +0x3
	struct {
		unsigned MaxSize : 11; // @0
		enum {
			None = 0,
			Extra1 = 1,
			Extra2 = 2,
		} Transactions : 2; // @11
		unsigned _reserved13_15 : 3; // @13
	} __attribute__ ((__packed__)) Packet; // +0x4
	u8 Interval; // +0x6
} __attribute__ ((__packed__));

/**
	\brief The string descriptor information.

	The string descriptor structure defined in the USB2.0 manual section 
	9.6.7.
*/
struct UsbStringDescriptor {
	u8 DescriptorLength; // +0x0
	enum DescriptorType DescriptorType : 8; // +0x1
	u16 Data[]; // +0x2 amount varies
} __attribute__ ((__packed__));

#ifdef __cplusplus
}
#endif

#endif // _USBD_DESCRIPTORS_H
/******************************************************************************
*	usbd/device.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	usbd/device.h contains a definition of a device structure used for 
*	storing devices and the device tree.
******************************************************************************/
#ifndef _DEVICE_H
#define _DEVICE_H

#ifdef __cplusplus
extern "C"
{
#endif


/** 
	\brief The maximum number of children a device could have, by implication, this is 
	the maximum number of ports a hub supports. 
	
	This is theoretically 255, as 8 bits are used to transfer the port count in
	a hub descriptor. Practically, no hub has more than 10, so we instead allow 
	that many. Increasing this number will waste space, but will not have 
	adverse consequences up to 255. Decreasing this number will save a little 
	space in the HubDevice structure, at the risk of removing support for an 
	otherwise valid hub. 
*/
#define MaxChildrenPerDevice 10
/** 
	\brief The maximum number of interfaces a device configuration could have. 

	This is theoretically 255 as one byte is used to transfer the interface 
	count in a configuration descriptor. In practice this is unlikely, so we 
	allow an arbitrary 8. Increasing this number wastes (a lot) of space in 
	every device structure, but should not have other consequences up to 255.
	Decreasing this number reduces the overheads of the UsbDevice structure, at
	the cost of possibly rejecting support for an otherwise supportable device. 
*/
#define MaxInterfacesPerDevice 8
/** 
	\brief The maximum number of endpoints a device could have (per interface). 
	
	This is theoretically 16, as four bits are used to transfer the endpoint 
	number in certain device requests. This is possible in practice, so we 
	allow that many. Decreasing this number reduces the space in each device
	structure considerably, while possible removing support for otherwise valid
	devices. This number should not be greater than 16.
*/
#define MaxEndpointsPerDevice 16

/**
	\brief Status of a USB device.

	Stores the status of a USB device. Statuses as defined in 9.1 of the USB2.0 
	manual.
*/
enum UsbDeviceStatus {
	Attached = 0,
	Powered = 1,
	Default = 2,
	Addressed = 3,
	Configured = 4,
};


/**
	\brief Status of a USB transfer.

	Stores the status of the last transfer a USB device did.
*/
enum UsbTransferError {
	NoError = 0,
	Stall = 1 << 1,
	BufferError = 1 << 2,
	Babble = 1 << 3,
	NoAcknowledge = 1 << 4,
	CrcError = 1 << 5,
	BitError = 1 << 6,
	ConnectionError = 1 << 7,
	AhbError = 1 << 8,
	NotYetError = 1 << 9,
	Processing = 1 << 31
};

/**
	\brief Start of a device specific data field.

	The first two words of driver data in a UsbDevice. The  DeviceDriver field 
	is a code which uniquely identifies the driver that set the driver data 
	field (i.e. the lowest driver in the stack above the USB driver). The 
	DataSize is the size in bytes of the device specific data field. 
*/
struct UsbDriverDataHeader {
	u32 DeviceDriver;
	u32 DataSize;
};

/**
	\brief Structure to store the details of a USB device that has been 
	detectd.

	Stores the details about a connected USB device. This is not directly part
	of the USB standard, and is instead a mechanism used to control the device
	tree.
*/
struct UsbDevice {
	u32 Number;

	UsbSpeed Speed;
	enum UsbDeviceStatus Status;
	volatile u8 ConfigurationIndex;
	u8 PortNumber;
	volatile enum UsbTransferError Error __attribute__((aligned(4)));
	
	// Generic device handlers
	/** Handler for detaching the device. The device driver should not issue further requests to the device. */
	void (*DeviceDetached)(struct UsbDevice *device) __attribute__((aligned(4)));
	/** Handler for deallocation of the device. All memory in use by the device driver should be deallocated. */
	void (*DeviceDeallocate)(struct UsbDevice *device);
	/** Handler for checking for changes to the USB device tree. Only hubs need handle with this. */
	void (*DeviceCheckForChange)(struct UsbDevice *device);
	/** Handler for removing a child device from this device. Only hubs need handle with this. */
	void (*DeviceChildDetached)(struct UsbDevice *device, struct UsbDevice *child);
	/** Handler for reseting a child device of this device. Only hubs need handle with this. */
	Result (*DeviceChildReset)(struct UsbDevice *device, struct UsbDevice *child);
	/** Handler for reseting a child device of this device. Only hubs need handle with this. */
	Result (*DeviceCheckConnection)(struct UsbDevice *device, struct UsbDevice *child);
	
	volatile struct UsbDeviceDescriptor Descriptor __attribute__((aligned(4)));
	volatile struct UsbConfigurationDescriptor Configuration __attribute__((aligned(4)));
	volatile struct UsbInterfaceDescriptor Interfaces[MaxInterfacesPerDevice] __attribute__((aligned(4)));
	volatile struct UsbEndpointDescriptor Endpoints[MaxInterfacesPerDevice][MaxEndpointsPerDevice] __attribute__((aligned(4)));
	struct UsbDevice *Parent __attribute__((aligned(4)));
	volatile void *FullConfiguration;
	volatile struct UsbDriverDataHeader *DriverData;
	volatile u32 LastTransfer;
};

#define InterfaceClassAttachCount 16

/**
	\brief Methods to attach a particular interface for a particular class.

	The class of the interface is the index into this array of methods. The
	array is populated by ConfigurationLoad().
*/

#ifdef __cplusplus
}
#endif

#endif // _DEVICE_H

/******************************************************************************
*	usbd/devicerequest.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	usbd/devicerequest.h contains a definition of the standard device 
*	request structure defined in USB2.0
******************************************************************************/
#ifndef _DEVICEREQUEST_H
#define _DEVICEREQUEST_H

#ifdef __cplusplus
extern "C"
{
#endif


/**
	\brief An encapsulated device request.

	A device request is a standard mechanism defined in USB2.0 manual section
	9.3 by which negotiations with devices occur. The request have a number of 
	parameters, and so are best implemented with a structure. As per usual,
	since this structure is arbitrary, we shall match Linux in the hopes of 
	achieving some compatibility.
*/
struct UsbDeviceRequest {
	u8 Type; // +0x0
	enum UsbDeviceRequestRequest {
		// USB requests
		GetStatus = 0,
		ClearFeature = 1,
		SetFeature = 3,
		SetAddress = 5,
		GetDescriptor = 6,
		SetDescriptor = 7,
		GetConfiguration = 8,
		SetConfiguration = 9,
		GetInterface = 10,
		SetInterface = 11,
		SynchFrame = 12,
		// HID requests
		GetReport = 1,
		GetIdle = 2,
		GetProtocol = 3,
		SetReport = 9,
		SetIdle = 10,
		SetProtocol = 11,
	} Request : 8; // +0x1
	u16 Value; // +0x2
	u16 Index; // +0x4
	u16 Length; // +0x6
} __attribute__ ((__packed__));


#ifdef __cplusplus
}
#endif

#endif // _DEVICEREQUEST_H

/******************************************************************************
*	usbd/pipe.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	usbd/pipe.h contains definitions relating to the USB pipe structure,
*	defined as part of the USB protocol. The precise details of this data 
*	structure are an implementation detail, matching Linux in this case to 
*	aid compatibility.
******************************************************************************/
#ifndef _USBD_PIPE_H
#define _USBD_PIPE_H

#ifdef __cplusplus
extern "C"
{
#endif


/**
	\brief Our implementation of the USB pipe defined in 10.5.1. 

	The UsbPipeAddress holds the address of a pipe. The USB standard defines 
	these as a software mechanism for communication between the USB driver and the
	host controller driver. We shall not have a concept of creating or destroying 
	pipes, as this is needless clutter, and simply just indicate the pipe by its
	physical properties. In other words, we identify the pipe by its physical 
	consequences on the USB. This is similar to Linux, and vastly reduces 
	complication, at the expense of requiring a little more sophistication on the
	sender's behalf. 
*/
struct UsbPipeAddress {
	UsbPacketSize MaxSize : 2; // @0
	UsbSpeed Speed : 2; // @2
	unsigned EndPoint : 4; // @4
	unsigned Device : 8; // @8
	UsbTransfer Type : 2; // @16
	UsbDirection Direction : 1; // @18
	unsigned _reserved19_31 : 13; // @19
} __attribute__ ((__packed__));


#ifdef __cplusplus
}
#endif

#endif // _USBD_PIPE_H
/******************************************************************************
*	usbd/usbd.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	usbd/usbd.h contains definitions relating to the generic USB driver. USB
*	is designed such that this driver's interface would be virtually the same
*	across all systems, and in fact its implementation varies little either.
******************************************************************************/

#ifndef USBD_H_
#define USBD_H_

#ifdef __cplusplus
extern "C"
{
#endif


/**
	\brief Performs all necessary operationg to start the USB driver.

	Initialises the USB driver by performing necessary interfactions with the
	host controller driver, and enumerating the device tree.
*/
Result UsbInitialise();

/**
	\brief Gets the descriptor for a given device.

	Gets the descriptor for a given device, using the index and language id 
	specified. The returned value is not longer than length.
*/
static Result UsbGetDescriptor(struct UsbDevice *device, enum DescriptorType 
    type, u8 index, u16 langId, void* buffer, u32 length, u32 minimumLength, u8 
recipient);

/**
	\brief Sends a control message synchronously to a given device.

	Sends a contorl message synchronously to a given device, then waits for 
	completion. If the timeout is reached returns ErrorTimeout. This puts
	device into an inconsistent state, so best not to use it until processing
	is unset.
*/
static Result UsbControlMessage(struct UsbDevice *device, struct UsbPipeAddress 
    pipe, void* buffer, u32 bufferLength,
	struct UsbDeviceRequest *request, u32 timeout);

/**
	\brief Allocates memory to a new device.

	Sets the value in the parameter device to the address of a new device 
	allocated on the heap, which then has appropriate default values.
*/
static Result UsbAllocateDevice(struct UsbDevice **device);

/*
	\brief Deallocates the memory and resources of a USB device.

	Recursively deallocates the device and all children. Deallocates any class
	specific data, as well as the device structure itself and releases the 
	device number.
*/
static void UsbDeallocateDevice(struct UsbDevice *device);

/**
	\brief Recursively enumerates a new device.

	Recursively enumerates a new device that has been allocated. This assigns
	an address, determines what the device is, and, if it is a hub, will 
	configure the device recursively look for new devices. If not, it will 
	configure the device with the default configuration.
*/
static Result UsbAttachDevice(struct UsbDevice *device);

/**
	\brief Returns a description for a device.

	Returns a description for a device. This is not read from the device, this 
	is just generated given by the driver.
*/
static const char* UsbGetDescription(struct UsbDevice *device);

/**
	\brief Returns a pointer to the root hub device.

	On a Universal Serial Bus, there exists a root hub. This if often a virtual
	device, and typically represents a one port hub, which is the physical 
	universal serial bus for this computer. It is always address 1. It is 
	present to allow uniform software manipulation of the universal serial bus 
	itself.
*/
struct UsbDevice *UsbGetRootHub();

/**
	\brief Scans the entire USB tree for changes.

	Recursively calls HubCheckConnection on all ports on all hubs connected to 
	the root hub.
*/
void UsbCheckForChange(void);


#ifdef __cplusplus
}
#endif

#endif
/******************************************************************************
*	device/hub.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	device/hub.h contains definitions relating to the USB hub device.
******************************************************************************/

#ifndef HUB_H_
#define HUB_H_

#ifdef __cplusplus
extern "C"
{
#endif


/**
	\brief The hub descriptor information.

	The hub descriptor structure defined in the USB2.0 manual section 
	11.23.2.1.
*/
struct HubDescriptor {
	u8 DescriptorLength; // +0x0
	enum DescriptorType DescriptorType : 8; // +0x1
	u8 PortCount; // +0x2
	struct {
		enum HubPortControl {
			Global = 0,
			Individual = 1,			
		} PowerSwitchingMode : 2; // @0
		bool Compound : 1; // @2
		enum HubPortControl OverCurrentProtection : 2; // @3
		unsigned ThinkTime : 2; // in +1*8FS units @5
		bool Indicators : 1; // @7
		unsigned _reserved8_15 : 8; // @8
	} __attribute__ ((__packed__)) Attributes; // +0x3
	u8 PowerGoodDelay; // +0x5
	u8 MaximumHubPower; // +0x6
	u8 Data[]; // +0x7 the data consists of n bytes describing port detatchability, followed by n bytes for compatiblity. n = roundup(ports/8).
} __attribute__ ((__packed__));

/**
	\brief Encapsulates the current status of a hub.

	The hub status structure defined in 11.24.2.6 of the USB2.0 
	standard.
*/
struct HubStatus {
	bool LocalPower : 1; // @0
	bool OverCurrent : 1; // @1
	unsigned _reserved2_15 : 14; // @2
} __attribute__ ((__packed__));

/**
	\brief Encapsulates the change in current status of a hub.

	The hub status change structure defined in 11.24.2.6 of the USB2.0 
	standard.
*/
struct HubStatusChange {
	bool LocalPowerChanged : 1; // @0
	bool OverCurrentChanged : 1; // @1
	unsigned _reserved2_15 : 14; // @2
} __attribute__ ((__packed__));

/**
	\brief Encapsulates the full status of a hub.

	The hub status structure defined in 11.24.2.6 of the USB2.0 standard.
*/
struct HubFullStatus {
	struct HubStatus Status;
	struct HubStatusChange Change;
} __attribute__ ((__packed__));
/**
	\brief Encapsulates the current status of a hub port.

	The hub port status structure defined in 11.24.2.7.1 of the USB2.0 
	standard.
*/
struct HubPortStatus {
	bool Connected : 1; // @0
	bool Enabled : 1; // @1
	bool Suspended : 1; // @2
	bool OverCurrent : 1; // @3
	bool Reset : 1; // @4
	unsigned _reserved5_7 : 3; // @5
	bool Power : 1; // @8
	bool LowSpeedAttatched : 1; // @9
	bool HighSpeedAttatched : 1; // @10
	bool TestMode : 1; // @11
	bool IndicatorControl : 1; // @12
	unsigned _reserved13_15 : 3; // @13
} __attribute__ ((__packed__));

/**
	\brief Encapsulates the change in current status of a hub port.

	The hub port status change structure defined in 11.24.2.7.2 of the USB2.0 
	standard.
*/
struct HubPortStatusChange {
	bool ConnectedChanged : 1; // @0
	bool EnabledChanged : 1; // @1
	bool SuspendedChanged : 1; // @2
	bool OverCurrentChanged : 1; // @3
	bool ResetChanged : 1; // @4
	unsigned _reserved5_15 : 11; // @5
} __attribute__ ((__packed__));

/**
	\brief Encapsulates the full status of a hub port.

	The hub port status structure defined in 11.24.2.7 of the USB2.0 standard.
*/
struct HubPortFullStatus {
	struct HubPortStatus Status;
	struct HubPortStatusChange Change;
} __attribute__ ((__packed__));

/**
	\brief A feature of a hub port.

	The feautres of a hub port that can be altered.
*/
enum HubPortFeature {
	FeatureConnection = 0,
	FeatureEnable = 1,
	FeatureSuspend = 2,
	FeatureOverCurrent = 3,
	FeatureReset = 4,
	FeaturePower = 8,
	FeatureLowSpeed = 9,
	FeatureHighSpeed = 10,
	FeatureConnectionChange = 16,
	FeatureEnableChange = 17,
	FeatureSuspendChange = 18,
	FeatureOverCurrentChange = 19,
	FeatureResetChange = 20,
};

/** The DeviceDriver field in UsbDriverDataHeader for hubs. */
#define DeviceDriverHub 0x48554230

/** 
	\brief Hub specific data.

	The contents of the driver data field for hubs. 
*/
struct HubDevice {	
	struct UsbDriverDataHeader Header;
	struct HubFullStatus Status;
	struct HubDescriptor *Descriptor;
	u32 MaxChildren;
	struct HubPortFullStatus PortStatus[MaxChildrenPerDevice];
	struct UsbDevice *Children[MaxChildrenPerDevice];
};

/**
	\brief A feature of a hub.

	The feautres of a hub that can be altered.
*/
enum HubFeature {
	FeatureHubPower = 0,
	FeatureHubOverCurrent = 1,
};


/**
	\brief Performs all necesary hub related initialisation.

	Loads a hub device and enumerates its children.
*/
static Result HubAttach(struct UsbDevice *device, u32 interfaceNumber);

/**
	\brief Resets a port on a hub.

	Resets a port on a hub. No validation.
*/
static Result HubPortReset(struct UsbDevice *device, u8 port);

/**
	\brief Checks the connection status of a port.

	Checks for a change in the connection status of a port. If it has changed
	performs necessary actions such as enumerating a new device or deallocating
	an old one.
*/
static Result HubCheckConnection(struct UsbDevice *device, u8 port);

/**
	\brief Checks all hubs for new devices.

	Recursively checks the hub tree for new devices being attached, and for old
	devices being removed.
*/
static void HubRecursiveCheck(struct UsbDevice *device);

#ifdef __cplusplus
}
#endif

#endif
/******************************************************************************
*	device/hid/hid.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	device/hid/hid.h contains definitions relating to generic human interface 
*	devices. Information about the hid reports is in device/hid/report.h.
******************************************************************************/

#ifndef HID_H_
#define HID_H_

#ifdef __cplusplus
extern "C"
{
#endif


/**
	\brief The human interface device descriptor information.

	The hid descriptor structure defined in the USB HID 1.11 manual in 6.2.1.
*/
struct HidDescriptor {
	u8 DescriptorLength; // +0x0
	enum DescriptorType DescriptorType : 8; // +0x1
	u16 HidVersion; // (bcd version) +0x2 
	enum HidCountry {
		CountryNotSupported = 0,
		Arabic = 1,
		Belgian = 2,
		CanadianBilingual = 3,
		CanadianFrench = 4,
		CzechRepublic = 5,
		Danish = 6,
		Finnish = 7,
		French = 8,
		German = 9,
		Greek = 10,
		Hebrew = 11,
		Hungary = 12,
		International = 13,
		Italian = 14,
		Japan = 15,
		Korean = 16,
		LatinAmerican = 17,
		Dutch = 18,
		Norwegian = 19,
		Persian = 20,
		Poland= 21,
		Portuguese = 22,
		Russian = 23,
		Slovakian = 24,
		Spanish = 25,
		Swedish = 26,
		SwissFrench = 27,
		SwissGerman = 28,
		Switzerland = 29,
		Taiwan = 30,
		TurkishQ = 31,
		EnglishUk = 32,
		EnglishUs = 33,
		Yugoslavian = 34,
		TurkishF = 35,
	} Countrycode : 8; // +0x4
	u8 DescriptorCount; // +0x5
	struct {
		enum DescriptorType Type : 8; // +0x0
		u16 Length; // +0x1
	} __attribute__ ((__packed__)) OptionalDescriptors[]; // +0x6 (a number of optional descriptors up to DescriptorCount)
} __attribute__ ((__packed__));

/**
	\brief The possible types of hid reports.

	The possible hid reports defined in the USB HID 1.11 manual in 7.2.1.
*/
enum HidReportType {
	Input = 1,
	Output = 2,
	Feature = 3,
};

/** The DeviceDriver field in UsbDriverDataHeader for hid devices. */
#define DeviceDriverHid 0x48494430

/** 
	\brief Hid specific data.

	The contents of the driver data field for hid devices. Chains to 
	allow a stacked driver. 
*/
struct HidDevice {
	struct UsbDriverDataHeader Header;
	struct HidDescriptor *Descriptor;
	struct HidParserResult *ParserResult;
	struct UsbDriverDataHeader *DriverData;

	// HID event handlers
	void (*HidDetached)(struct UsbDevice* device);
	void (*HidDeallocate)(struct UsbDevice* device);
};

#define HidUsageAttachCount 10

/**
	\brief Methods to attach an interface of particular hid desktop usage.

	The application desktop usage of the interface is the index into this array
	of methods. The array is populated by ConfigurationLoad().
*/
static Result (*HidUsageAttach[HidUsageAttachCount])(struct UsbDevice *device, 
u32 interfaceNumber);

/**
	\brief Retrieves a hid report.

	Performs a hid get report request as defined in  in the USB HID 1.11 manual
	in 7.2.1. 
*/
static Result HidGetReport(struct UsbDevice *device, enum HidReportType 
    reportType, u8 reportId, u8 interface, u32 bufferLength, void* buffer);

/**
	\brief Sends a hid report.

	Performs a hid set report request as defined in  in the USB HID 1.11 manual
	in 7.2.2. 
*/
static Result HidSetReport(struct UsbDevice *device, enum HidReportType 
    reportType, u8 reportId, u8 interface, u32 bufferLength, void* buffer);

/**
	\brief Updates the device with the values of a report.

	Writes back the current values of a report in memory to the device. 
	Implemented using HidSetReport, not interrupts.
*/
static Result HidWriteDevice(struct UsbDevice *device, u8 report);

/**
	\brief Updates a report with the values from the device.

	Reads the current values of a report from the device into memory. Implemented
	using HidGetReport not interrupts.
*/
static Result HidReadDevice(struct UsbDevice *device, u8 report);

/**
	\brief Enumerates a device as a HID device.

	Checks a device to see if it appears to be a HID device, and, if so, loads
	its hid and report descriptors to see what it can do.
*/
static Result HidAttach(struct UsbDevice *device, u32 interfaceNumber);

#ifdef __cplusplus
}
#endif

#endif
/******************************************************************************
*	device/hid/keyboard.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	device/hid/keyboard.h contains definitions relating to keyboards.
******************************************************************************/

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#ifdef __cplusplus
extern "C"
{
#endif



/** 
	\brief The current state of keyboard modifier keys.

	Encapsulates the current state of the keyboard modifier keys. Strucutre 
	mimics the most common keyboard ordering.
*/
struct KeyboardModifiers {
	bool LeftControl : 1; // @0
	bool LeftShift : 1; // @1
	bool LeftAlt : 1; // @2
	bool LeftGui : 1; // the 'windows' key @3
	bool RightControl : 1;  // @4
	bool RightShift : 1; // @5
	bool RightAlt : 1; // 'alt gr' @6
	bool RightGui : 1; // @7
} __attribute__ ((__packed__));

/** 
	\brief The current state of keyboard leds.

	Encapsulates the current state of the keyboard leds. Strucutre mimics the 
	most common lights and ordering. Not all keyboards may support all lights.
*/
struct KeyboardLeds {
	bool NumberLock : 1;
	bool CapsLock : 1;
	bool ScrollLock : 1;
	bool Compose : 1;
	bool Kana : 1;
	bool Power : 1;
	bool Mute : 1;
	bool Shift : 1;
} __attribute__ ((__packed__));

/** The DeviceDriver field in UsbDriverDataHeader for keyboard devices. */
#define DeviceDriverKeyboard 0x4B424430
/** The maximum number of keys a keyboard can report at once. Should be 
	multiple of 2. */
#define KeyboardMaxKeys 6

/** 
	\brief Keyboard specific data.

	The contents of the driver data field for keyboard devices. Placed in
	HidDevice, as this driver is built atop that.
*/
struct KeyboardDevice {
	/** Standard driver data header. */
	struct UsbDriverDataHeader Header;
	/** Internal - Index in keyboard arrays. */
	u32 Index;
	/** Number of keys currently held down. */
	u32 KeyCount;
	/** Keys currently held down. */
	u16 Keys[KeyboardMaxKeys];
	/** Modifier keys currently held down. */
	struct KeyboardModifiers Modifiers;
	/** Which LEDs this keyboard supports. */
	struct KeyboardLeds LedSupport;
	/** Which fields in the LED report are for what LEDs. */
	struct HidParserField *LedFields[8];
	/** Which fields in the Input report are for what modifiers and keys. */
	struct HidParserField *KeyFields[8 + 1];
	/** The LED report. */
	struct HidParserReport *LedReport;
	/** The input report. */
	struct HidParserReport *KeyReport;
};

/**
	\brief Enumerates a device as a keyboard.

	Checks a device already checked by HidAttach to see if it appears to be a 
	keyboard, and, if so, builds up necessary information to enable the 
	keyboard methods.
*/
static Result KeyboardAttach(struct UsbDevice *device, u32 interface);

/**
	\brief Returns the number of keyboards connected to the system.	
*/
int KeyboardCount(void);

/**
	\brief Sets the keyboard LEDs to the state given in leds.

	Sets the keyboard LEDs to the state given in leds. Unimplemented LEDs are 
	ignored silently. 
*/
static Result KeyboardSetLeds(u32 keyboardAddress, struct KeyboardLeds leds); 

/**
	\brief Gets a list of available keyboard LEDs.

	Reads the availablility of keyboard LEDs from the report descriptor. LEDs 
	that are present are set to 1, and those than are not are set to 0.
*/
struct KeyboardLeds KeyboardGetLedSupport(u32 keyboardAddress); 

/**
	\brief Checks a given keyboard.

	Reads back the report from a given keyboard and parses it into the internal
	fields. These can be accessed with KeyboardGet... methods
*/
Result KeyboardPoll(unsigned int keyboardAddress);

/**
	\brief Reads the modifier keys from a keyboard.

	Reads back the state of the modifier keys from the last sucessfully 
	received report. Zeros out by default.
*/
static struct KeyboardModifiers KeyboardGetModifiers(u32 keyboardAddress);

/**
	\brief Returns the number of keys currently held down.

	Reads back the number of keys that were held down in the last report. If 
	the keyboard reaches its key limit, this reports the last sensible report 
	received.
*/
static u32 KeyboardGetKeyDownCount(u32 keyboardAddress);

/**
	\brief Returns whether or not a particular key is held down.

	Reads back whether or not a key was held on the last successfully received 
	report.
*/
static bool KeyboadGetKeyIsDown(u32 keyboardAddress, u16 key);

/**
	\brief Returns the nth key that is held down.

	Reads back the number of the nth key that was held down in the last 
	successfully received report.
*/
u16 KeyboardGetKeyDown(u32 keyboardAddress, u32 index);

/** 
	\brief Returns the device address of the nth connected keyboard.

	Keyboards that are connected are stored in an array, and this method 
	retrieves the nth item from that array. Returns 0 on error.
*/
unsigned int KeyboardGetAddress(u32 index);


#ifdef __cplusplus
}
#endif

#endif
/******************************************************************************
*	device/hid/mouse.h
*	 by Steve White 
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	device/hid/mouse.h contains definitions relating to mouses.
******************************************************************************/

#ifndef MOUSE_H_
#define MOUSE_H_

#ifdef __cplusplus
extern "C"
{
#endif


/** The DeviceDriver field in UsbDriverDataHeader for mouse devices. */
#define DeviceDriverMouse 0x4B424431
/** The maximum number of keys a mouse can report at once. Should be 
	multiple of 2. */
#define MouseMaxKeys 6

enum MouseDeviceButton {
	MouseDeviceButtonLeft,
	MouseDeviceButtonRight,
	MouseDeviceButtonMiddle,
	MouseDeviceButtonSide,
	MouseDeviceButtonExtra,
};

/** 
	\brief Mouse specific data.

	The contents of the driver data field for mouse devices. Placed in
	HidDevice, as this driver is bult atop that.
*/
struct MouseDevice {
	/** Standard driver data header. */
	struct UsbDriverDataHeader Header;
	/** Internal - Index in mouse arrays. */
	u32 Index;

	u8 buttonState;
	s16 mouseX;
	s16 mouseY;
	s16 wheel;

	/** The input report. */
	struct HidParserReport *MouseReport;
};

/**
	\brief Enumerates a device as a mouse.

	Checks a device already checked by HidAttach to see if it appears to be a 
	mouse, and, if so, bulds up necessary information to enable the 
	mouse methods.
*/
static Result MouseAttach(struct UsbDevice *device, u32 interface);

/**
	\brief Returns the number of mouses connected to the system.	
*/
static u32 MouseCount();

/**
	\brief Checks a given mouse.

	Reads back the report from a given mouse and parses it into the internal
	fields. These can be accessed with MouseGet... methods
*/
static Result MousePoll(u32 mouseAddress);

/** 
	\brief Returns the device address of the nth connected mouse.

	Mouses that are connected are stored in an array, and this method 
	retrieves the nth item from that array. Returns 0 on error.
*/
static u32 MouseGetAddress(u32 index);

/**
	\brief Returns the current X coordinate of the mouse
*/
static s16 MouseGetPositionX(u32 mouseAddress);

/**
	\brief Returns the current Y coordinate of the mouse
*/
static s16 MouseGetPositionY(u32 mouseAddress);

/**
	\brief Returns the current wheel value of the mouse
*/
static s16 MouseGetWheel(u32 mouseAddress);

/**
	\brief Returns the current X and Y coordinates of the mouse

	X is in the high 16 bits, Y is in the low 16 bits
*/
static u32 MouseGetPosition(u32 mouseAddress);

/**
	\brief Returns the current button state of the mouse

	First bit : Left button
	Second bit: Right button
	Third bit : Middle button
	Fourth bit: Side button
	Fifth bit : Extra button
*/
static u8 MouseGetButtons(u32 mouseAddress);

/**
	\brief Returns whether or not a particular mouse button is pressed.

	Reads back whether or not a mouse button was pressed on the last 
	successfully received report.
*/
static bool MouseGetButtonIsPressed(u32 mouseAddress, enum MouseDeviceButton 
button);


#ifdef __cplusplus
}
#endif

#endif
/******************************************************************************
*	device/hid/report.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	device/hid/report.h contains definitions relating to human interface
*	device reports, used to communicate the functionality of any given hid 
*	device.
******************************************************************************/

#ifndef REPORT_H_
#define REPORT_H_


#ifdef __cplusplus
extern "C"
{
#endif

/**
	\brief An item in an hid report descriptor.

	The hid report item sturcture defined in the USB HID 1.11 manual in
	6.2.2.2. There are two such types defined here, long and short. Since long
	is based upon short. So we can resuse the structure to this effect.
*/
struct HidReportItem {
	unsigned Size : 2; // Value is 1 << (size - 1) @0 
	enum HidReportTag {
		TagMainInput = 0x20,
		TagMainOutput = 0x24,
		TagMainFeature = 0x2c,
		TagMainCollection = 0x28,
		TagMainEndCollection = 0x30,
		TagGlobalUsagePage = 0x1,
		TagGlobalLogicalMinimum = 0x5,
		TagGlobalLogicalMaximum = 0x9,
		TagGlobalPhysicalMinimum = 0xd,
		TagGlobalPhysicalMaximum = 0x11,
		TagGlobalUnitExponent = 0x15,
		TagGlobalUnit = 0x19,
		TagGlobalReportSize = 0x1d,
		TagGlobalReportId = 0x21,
		TagGlobalReportCount = 0x25,
		TagGlobalPush = 0x29,
		TagGlobalPop = 0x2d,
		TagLocalUsage = 0x2,
		TagLocalUsageMinimum = 0x6,
		TagLocalUsageMaximum = 0xa,
		TagLocalDesignatorIndex = 0xe,
		TagLocalDesignatorMinimum = 0x12,
		TagLocalDesignatorMaximum = 0x16,
		TagLocalStringIndex = 0x1e,
		TagLocalStringMinimum = 0x22,
		TagLocalStringMaximum = 0x26,
		TagLocalDelimiter = 0x2a,
		TagLong = 0x3f,
	} Tag : 6; // @2
} __attribute__ ((__packed__));

/**
	\brief A main input, output or feature in an hid report descriptor.

	The hid main input, output or feature item sturcture defined in the USB HID
	1.11 manual in 6.2.2.4. 
*/
struct HidMainItem {
	bool Constant : 1; // Data=0,Constant=1 @0
	bool Variable : 1; // Array=0,Variable=1 @1	
	bool Relative : 1; // Absolute=0,Relative=1 @2
	bool Wrap : 1; // NoWrap=0,Wrap=1 @3
	bool NonLinear : 1; // Linear=0,NonLinear=1 @4
	bool NoPreferred : 1; // PreferredState=0,NoPreferred=1 @5
	bool Null : 1; // NoNull=0,NullState=1 @6
	bool Volatile : 1; // NonVolatile=0,Volatile=1 Inputs cannot be volatile @7
	bool BufferedBytes : 1; // BitField=0,BufferedBytes=1 @8
	unsigned _reserved9_31 : 23; // @9
} __attribute__ ((__packed__));

/**
	\brief A main collection index in an hid report descriptor.

	The hid main collection index used in the collection item defined in the 
	USB HID 1.11 manual in 6.2.2.4. 
*/
enum HidMainCollection {
	Physical = 0,
	Application = 1,
	Logical = 2,
	Report = 3,
	NamedArray = 4,
	UsageSwitch = 5,
	UsageModifier = 6,
};

/**
	\brief Values of the hid page usage field in a report.

	Values that the hid page usage can take in a hid report descriptor. Defined
	in section 3 table 1 of the HID 1.11 usage tables.
*/
enum HidUsagePage {
	Undefined = 0,
	GenericDesktopControl = 1,
	SimulationControl = 2,
	VrControl = 3,
	SportControl = 4,
	GameControl = 5,
	GenericDeviceControl = 6,
	KeyboardControl = 7,
	Led = 8,
	Button = 9,
	Ordinal = 10,
	Telephony = 11,
	Consumer = 12,
	Digitlizer = 13,
	PidPage = 15,
	Unicode = 16,
	_HidUsagePage = 0xffff,
};

/**
	\brief Values of the hid desktop page usage in a report.

	Values that usage numbers in the desktop page represent. Defined in 
	section 4 table 6 of the HID 1.11 usage tables. Only items below 0x48 are 
	included here for brevity.
*/
enum HidUsagePageDesktop {
	DesktopPoint = 1,
	DesktopMouse = 2,
	DesktopJoystick = 4,
	DesktopGamePad = 5,
	DesktopKeyboard = 6,
	DesktopKeypad = 7,
	DesktopMultiAxisController = 8,
	DesktopTablePcControl = 9,
	DesktopX = 0x30,
	DesktopY = 0x31,
	DesktopZ = 0x32,
	DesktopRX = 0x33,
	DesktopRY = 0x34,
	DesktopRZ = 0x35,
	DesktopSlider = 0x36,
	DesktopDial = 0x37,
	DesktopWheel = 0x38,
	DesktopHatSwitch = 0x39,
	DesktopCountedBuffer = 0x3a,
	DesktopByteCount = 0x3b,
	DesktopMotionWakeup = 0x3c,
	DesktopStart = 0x3d,
	DesktopSelect = 0x3e,
	DesktopVX = 0x40,
	DesktopVY = 0x41,
	DesktopVZ = 0x42,
	DesktopVbrX = 0x43,
	DesktopVbrY = 0x44,
	DesktopVbrZ = 0x45,
	DesktopVno = 0x46,
	DesktopFeatureNotification = 0x47,
	DesktopResolutionMultiplier = 0x48,
	_HidUsagePageDesktopDummy = 0xffff,
};

/**
	\brief Values of the hid keyboard page usage in a report.

	Values that usage numbers in the keyboard page represent. Defined in 
	section 10 table 12 of the HID 1.11 usage tables. Key definitions are not
	included here, as this driver, being deliberately akward, does not handle 
	them.
*/
enum HidUsagePageKeyboard {
	KeyboardErrorRollOver = 1,
	KeyboardPostFail = 2,
	KeyboardErrorUndefined = 3,
	KeyboardLeftControl = 0xe0,
	KeyboardLeftShift = 0xe1,
	KeyboardLeftAlt = 0xe2,
	KeyboardLeftGui = 0xe3,
	KeyboardRightControl = 0xe4,
	KeyboardRightShift = 0xe5,
	KeyboardRightAlt = 0xe6,
	KeyboardRightGui = 0xe7,
	_HidUsagePageKeyboard = 0xffff,
};

/**
	\brief Values of the hid led page usage in a report.

	Values that usage numbers in the keyboard page represent. Defined in 
	section 11 table 13 of the HID 1.11 usage tables.
*/
enum HidUsagePageLed {
	LedNumberLock = 1,
	LedCapsLock = 2,
	LedScrollLock = 3,
	LedCompose = 4,
	LedKana = 5,
	LedPower = 6,
	LedShift = 7,
	LedMute = 9,
	_HidUsagePageLed = 0xffff,
};

/**
	\brief A full HID usage tag.

	A full HID usage with both the page and the tag represented in the order it 
	can appear in HID report descriptors.
*/
struct HidFullUsage {
	union {
		enum HidUsagePageDesktop Desktop : 16;
		enum HidUsagePageKeyboard Keyboard : 16;
		enum HidUsagePageLed Led : 16;
	};
	enum HidUsagePage Page : 16;
} __attribute__ ((__packed__));

/** 
	\brief A HID field units declaration.

	The units of a HID field. This declaration allows the defintion of many SI 
	units.
*/
struct HidUnit {
	enum {
		SystemNone = 0,
		/** Centimeter, Gram, Second, Kelvin, Ampere, Candela */
		StandardLinear = 1,
		/** Radian, Gram, Second, Kelvin, Ampere, Candela */
		StandardRotation = 2,
		/** Inch, Slug, Second, Fahrenheit, Ampere, Candela */
		EnglishLinear = 3,
		/** Degree, Slug, Second, Fahrenheit, Ampere, Candela */
		EnglishRotation = 4,
	} System : 4;
	signed Length : 4;
	signed Mass : 4;
	signed Time : 4;
	signed Temperature : 4;
	signed Current : 4;
	signed LuminousIntensity : 4;
	unsigned _reserved28_31 : 4;
} __attribute__ ((__packed__));

/**
	\brief A parsed report field, with value.

	A representation of a fully parsed report field complete with value for 
	easy retrieval and setting of values.
*/
struct HidParserField {
	/** Size in bits of this field. For arrays, this is per element. */
	u8 Size;
	/** Offset of this field into the report in bits */
	u8 Offset;
	/** Array fields have a number of individual fields. */
	u8 Count;
	/** Attributes of this field */
	struct HidMainItem Attributes __attribute__((aligned(4)));
	/** Usage of this field. For array elements, this is the first usage, and 
		it is assumed sequential values have sequential usage. */
	struct HidFullUsage Usage;
	/** Usage of the physical connection this device is in, if present. */
	struct HidFullUsage PhysicalUsage;
	/** The minimum value of this field. */
	s32 LogicalMinimum;
	/** The maximum value of this field. */
	s32 LogicalMaximum;	
	/** The minimum physical quantity represented by this field. */
	s32 PhysicalMinimum;
	/** The maximum physical quantity represented by this field. */
	s32 PhysicalMaximum;
	/** The units of this field's physical quantity. */
	struct HidUnit Unit;
	/** The base 10 exponenet of this field's physical quantity. */
	s32 UnitExponent;
	/** Current value of this field. This is the logical value. The value is 
		not sign extended. */
	union {
		u8 U8;
		s8 S8;
		u16 U16;
		s16 S16;
		u32 U32;
		s32 S32;
		bool Bool;
		void* Pointer;
	} Value;
};

/**
	\brief A parsed report, with values.

	A representation of a fully parsed report complete with value fields for 
	easy retrieval and setting of values.
*/
struct HidParserReport {
	/** Which report this is in the parser result. */
	u8 Index;
	/** There can be multiple fields in each report */
	u8 FieldCount;
	/** Report can have an ID. If not we use 0. */
	u8 Id;
	/** The type of this report. */
	enum HidReportType Type;
	/** Length of this report in bytes. */
	u8 ReportLength;
	/** The last report received (if not NULL). */
	u8 *ReportBuffer;
	/** Store the fields sequentially */
	struct HidParserField Fields[] __attribute__((aligned(4)));
};

/**
	\brief A parsed report descriptor, with values.

	A representation of a fully parsed report descriptor complete with value 
	fields for easy retrieval and setting of values.
*/
struct HidParserResult {
	/** Each report descriptor has an application collection with a usage */
	struct HidFullUsage Application; 
	/** There can be multiple reports */
	u8 ReportCount;
	/** The interface number that HID is available on. */
	u8 Interface;
	/** Store a pointer to each report. */
	struct HidParserReport *Report[] __attribute__((aligned(4)));
};

/**
	\brief Retrieves a value from a field.

	Reads the current value at a given index in a HID field.
*/
static s32 HidGetFieldValue(struct HidParserField *field, u32 index);


#ifdef __cplusplus
}
#endif

#endif
/******************************************************************************
*	hcd/hcd.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	hcd/hcd.h contains definitions relating to the host controller driver of
*	the USB implementation.
******************************************************************************/

#ifndef HCD_H_
#define HCD_H_

#ifdef __cplusplus
extern "C"
{
#endif



/**
	\brief Intialises the host controller driver for this hardware.

	Initialises the core on whatever hardware is in use. 
*/
static Result HcdInitialise();

/** 
	\brief Starts the host controller driver working.

	Starts the host controller driver working. It should start processing 
	communications and be ready for commands.
*/
static Result HcdStart();

/** 
	\brief Stops the host controller driver working.

	Stops the host controller driver working. This should close all connections
	and return the Hcd to a state such that a subsequent call to StopHcd would 
	restart everything. This could be to update parameters, fix a fault, etc.
*/
static Result HcdStop();

/** 
	\brief Uninitialises the host controller driver.

	Unitialises the host controller driver. This should be called when the 
	driver is to be complete removed. It should power off any hardware, and 
	and return the driver to a point such that a subsequent call to 
	InitialiseHcd would restart it.
*/
static Result HcdDeinitialise();

/**
	\brief Sends a control message to a device.

	Sends a control message to a device. Handles all necessary channel creation
	and other processing. The sequence of a control transfer is defined in the 
	USB 2.0 manual section 5.5. The host sends a setup packet (request) then 
	zero or more data packets are sent or received (to buffer, max length 
	bufferLength) and finally a status message is sent to conclude the 
	transaction. Packets larger than pipe.MaxSize are split. For low speed 
	devices pipe.MaxSize must be Bits8, and Bits64 for high speed. Low and full
	speed transactions are always split.
*/
static Result HcdSumbitControlMessage(struct UsbDevice *device, struct 
    UsbPipeAddress pipe, void* buffer, u32 bufferLength,
	struct UsbDeviceRequest *request);


#ifdef __cplusplus
}
#endif

#endif
/******************************************************************************
*	platform/platform.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	platform/platform.h contains definitions pertaining to the platform that
*	the system will run on.
******************************************************************************/

#ifndef _PLATFORM_H
#define _PLATFORM_H

#ifdef __cplusplus
extern "C"
{
#endif


/**
	\brief Allocates memory of specified length. 

	Allocates memory of length at least length to the driver and returns the 
	address. If MEM_INTERNAL_MANAGER is defined, then all platforms must
	provide an implementation which calls MemoryReserve. Can return NULL on
	error.
*/
static void* MemoryAllocate(u32 length);
/**
	\brief Deallocates memory of specified address, previously allocated by
	MemoryAllocate. 

	Deallocates memory of the specified address that was previously allocated
	by MemoryAllocate. If MEM_INTERNAL_MANAGER is defined, then all platforms
	must provide an implementation. Calling with an address not received from
	MemoryAllocate produces undefined results.
*/
static void MemoryDeallocate(void* address);
/**
	\brief Notifies the system of memory usage. 

	Notifies the parent system of an unavoidable memory usage. This is 
	typically used for memory mapped IO systems, in which certain addresses
	have special meaning. It is up to the parent system to implement whatever
	must be done. The return value should be a virtual address that maps to 
	the requested physical address, or NULL on error. If MEM_NO_RESERVE is 
	defined, a dummy implementation is created.
*/
static void* MemoryReserve(u32 length, void* physicalAddress);
/**
	\brief Copies chunks of memory. 

	Copies length bytes from source to destinatoin. If either source or 
	destination are null, should not copy anything.
*/
static void MemoryCopy(void* destination, void* source, u32 length);

#ifdef NO_LOG
#define LOG(x)
#define LOGL(x, len) 
#define LOGF(x, ...) 
#define LOGFL(x, len, ...) 
#else
/**
	\brief Notifies the user of progress. 

	Notifies the parent system of progress loading the driver. Messages may be 
	displayed to a semi-technically competant user.
static void LogPrint(const char* message, u32 messageLength);
*/
/**
	\brief Notifies the user of progress. 

	Prints our a formatted string. Uses all the formtting options in printf. 
	Implemented in platform.c, by calling LogPrint. Messages truncated to 160
	characters.
static void LogPrintF(char* format, u32 formatLength, ...);
*/

#define LOG(x)
#define LOGL(x, len)
#define LOGF(x, ...)
#define LOGFL(x, len, ...)
#endif
#ifdef DEBUG
#define LOG_DEBUG(x) LOG(x)
#define LOG_DEBUGL(x, len) LOGL(x, len)
#define LOG_DEBUGF(x, ...) LOGF(x, __VA_ARGS__)
#define LOG_DEBUGFL(x, len, ...) LOGFL(x, len, __VA_ARGS__)
#else
#define LOG_DEBUG(x) 
#define LOG_DEBUGL(x, len) 
#define LOG_DEBUGF(x, ...) 
#define LOG_DEBUGFL(x, len, ...) 
#endif

/**
	\brief Turns on the USB host controller.

	Notifies the parent system that the USB contorller now requires power. 
*/
static Result PowerOnUsb();
/**
	\brief Turns on the USB host controller.

	Notifies the parent system that the USB contorller no longer requires power. 
*/
static void PowerOffUsb();

/**
	\brief Delays for delay microseconds.

	Delays for a number of microseconds. 	
*/
static void MicroDelay(u32 delay);


#ifdef ARM
#	ifdef ARM_V6
#		ifdef BROADCOM_2835
#		endif // BROADCOM_2835
#	else
#	error Unrecognised ARM Version
#	endif // ARM_V6
#else
#error Unrecognised Processor Family
#endif // ARM

#ifdef __cplusplus
}
#endif

#endif
/******************************************************************************
*	hcd/dwc/designware.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	hcd/dwc/designware.h contains definitions pertaining to the DesignWare® 
*	Hi-Speed USB 2.0 On-The-Go (HS OTG) Controller.
*
*	THIS SOFTWARE IS NOT AFFILIATED WITH NOR ENDORSED BY SYNOPSYS IP.
******************************************************************************/
#ifndef _HCD_DWC_DESIGNWARE_H
#define _HCD_DWC_DESIGNWARE_H

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef HCD_DESIGNWARE_20

#define ReceiveFifoSize 20480 /* 16 to 32768 */
#define NonPeriodicFifoSize 20480 /* 16 to 32768 */
#define PeriodicFifoSize 20480 /* 16 to 32768 */
#define ChannelCount 16
#define RequestTimeout 5000

/**
	\brief The addresses of all core registers used by the HCD.

	The address offsets of all core registers used by the DesignWare® Hi-Speed 
	USB 2.0 On-The-Go (HS OTG) Controller. Used in write and read throughs.
*/
enum CoreRegisters {
	RegOtgControl = 0x0,
	RegOtgInterrupt = 0x4,
	RegAhb = 0x8,
	RegUsb = 0xc,
	RegReset = 0x10,
	RegInterrupt = 0x14,
	RegInterruptMask = 0x18,
	RegReceivePeek = 0x1c,
	RegReceivePop = 0x20,
	RegReceiveSize = 0x24,
	RegNonPeriodicFifoSize = 0x28,
	RegNonPeriodicFifoStatus = 0x2c,
	RegI2cControl = 0x30,
	RegPhyVendorControl = 0x34,
	RegGpio = 0x38,
	RegUserId = 0x3c,
	RegVendorId = 0x40,
	RegHardware = 0x44,
	RegLowPowerModeConfiguation = 0x48,
#ifdef BROADCOM_2835
	RegMdioControl = 0x80,
	RegMdioRead = 0x84,
	RegMdioWrite = 0x84,
	RegMiscControl = 0x88,
#endif
	RegPeriodicFifoSize = 0x100,
	RegPeriodicFifoBase = 0x104,
	RegPower = 0xe00,
};

/**
	\brief The addresses of all core registers used by the HCD.

	The address offsets of all host registers used by the DesignWare® Hi-Speed 
	USB 2.0 On-The-Go (HS OTG) Controller. Used in write and read throughs.
*/
enum HostRegisters {
	RegHostConfig = 0x400,
	RegHostFrameInterval = 0x404,
	RegHostFrameNumber = 0x408,
	RegHostFifoStatus = 0x410,
	RegHostInterrupt = 0x414,
	RegHostInterruptMask = 0x418,
	RegHostFrameList = 0x41c,
	RegHostPort = 0x440,
	RegHostChannelBase = 0x500,
	RegHostChannelCharactristicBase = RegHostChannelBase + 0x0,
	RegHostChannelSplitControl = RegHostChannelBase + 0x4,
	RegHostChannelInterrupt = RegHostChannelBase + 0x8,
	RegHostChannelInterruptMask = RegHostChannelBase + 0xc,
	RegHostChannelTransferSize = RegHostChannelBase + 0x10,
	RegHostChannelDmaAddress = RegHostChannelBase + 0x14,
	RegHostChannelDmaBuffer = RegHostChannelBase + 0x1c,
};

/**
	\brief The interrupts in the core register.

	Contains the core interrutps that controls the DesignWare® Hi-Speed USB 2.0
	On-The-Go (HS OTG) Controller.
*/
struct CoreInterrupts {
	bool CurrentMode : 1; // @0
	bool ModeMismatch : 1; // @1
	bool Otg : 1; // @2
	bool DmaStartOfFrame : 1; // @3
	bool ReceiveStatusLevel : 1; // @4
	bool NpTransmitFifoEmpty : 1; // @5
	bool ginnakeff:1; // @6
	bool goutnakeff:1; // @7
	bool ulpick:1; // @8
	bool I2c : 1; // @9
	bool EarlySuspend : 1; // @10
	bool UsbSuspend : 1; // @11
	bool UsbReset : 1; // @12
	bool EnumerationDone : 1; // @13
	bool IsochronousOutDrop : 1; // @14
	bool eopframe:1; // @15
	bool RestoreDone : 1; // @16
	bool EndPointMismatch : 1; // @17
	bool InEndPoint : 1; // @18
	bool OutEndPoint : 1; // @19
	bool IncompleteIsochronousIn : 1; // @20
	bool IncompleteIsochronousOut : 1; // @21
	bool fetsetup:1; // @22
	bool ResetDetect : 1; // @23
	bool Port : 1; // @24
	bool HostChannel : 1; // @25
	bool HpTransmitFifoEmpty : 1; // @26
	bool LowPowerModeTransmitReceived : 1; // @27
	bool ConnectionIdStatusChange : 1; // @28
	bool Disconnect : 1; // @29
	bool SessionRequest : 1; // @30
	bool Wakeup : 1; // @31
} __attribute__ ((__packed__));

/**
	\brief The interrupts in the channel registers.

	Contains the interrupts that controls the channels of the DesignWare® 
	Hi-Speed USB 2.0 On-The-Go (HS OTG) Controller.
*/
struct ChannelInterrupts {
	bool TransferComplete : 1; // @0
	bool Halt : 1; // @1
	bool AhbError : 1; // @2
	bool Stall : 1; // @3
	bool NegativeAcknowledgement : 1; // @4
	bool Acknowledgement : 1; // @5
	bool NotYet : 1; // @6
	bool TransactionError : 1; // @7
	bool BabbleError : 1; // @8
	bool FrameOverrun : 1; // @9
	bool DataToggleError : 1; // @10
	bool BufferNotAvailable : 1; // @11
	bool ExcessiveTransmission : 1; // @12
	bool FrameListRollover : 1; // @13
	unsigned _reserved14_31 : 18; // @14
} __attribute__ ((__packed__));

/**
	\brief Contains the core global registers structure that control the HCD.

	Contains the core global registers structure that controls the DesignWare®
	Hi-Speed USB 2.0 On-The-Go (HS OTG) Controller.
*/
__attribute__ ((__packed__))
struct CoreGlobalRegs {
	volatile struct {
		volatile bool sesreqscs : 1;
		volatile bool sesreq : 1;
		volatile bool vbvalidoven:1;
		volatile bool vbvalidovval:1;
		volatile bool avalidoven:1;
		volatile bool avalidovval:1;
		volatile bool bvalidoven:1;
		volatile bool bvalidovval:1;
		volatile bool hstnegscs:1;
		volatile bool hnpreq:1;
		volatile bool HostSetHnpEnable : 1;
		volatile bool devhnpen:1;
		volatile unsigned _reserved12_15:4;
		volatile bool conidsts:1;
		volatile unsigned dbnctime:1;
		volatile bool ASessionValid : 1;
		volatile bool BSessionValid : 1;
		volatile unsigned OtgVersion : 1;
		volatile unsigned _reserved21:1;
		volatile unsigned multvalidbc:5;
		volatile bool chirpen:1;
		volatile unsigned _reserved28_31:4;
	} __attribute__ ((__packed__)) OtgControl; // +0x0
	volatile struct {
		volatile unsigned _reserved0_1 : 2; // @0
		volatile bool SessionEndDetected : 1; // @2
		volatile unsigned _reserved3_7 : 5; // @3
		volatile bool SessionRequestSuccessStatusChange : 1; // @8
		volatile bool HostNegotiationSuccessStatusChange : 1; // @9
		volatile unsigned _reserved10_16 : 7; // @10
		volatile bool HostNegotiationDetected : 1; // @17
		volatile bool ADeviceTimeoutChange : 1; // @18
		volatile bool DebounceDone : 1; // @19
		volatile unsigned _reserved20_31 : 12; // @20
	} __attribute__ ((__packed__)) OtgInterrupt; // +0x4
	volatile struct {
		volatile bool InterruptEnable : 1; // @0
#ifdef BROADCOM_2835
		// In accordance with the SoC-Peripherals manual, broadcom redefines 
		// the meaning of bits 1:4 in this structure.
		volatile enum {
			Length4 = 0,
			Length3 = 1,
			Length2 = 2,
			Length1 = 3,
		} AxiBurstLength : 2; // @1
		volatile unsigned _reserved3 : 1; // @3
		volatile bool WaitForAxiWrites : 1; // @4
#else
		volatile enum {
			Single,
			Incremental,
			Incremental4 = 3,
			Incremental8 = 5,
			Incremental16 = 7,
		} DmaBurstType : 4; // @1
#endif
		volatile bool DmaEnable : 1; // @5
		volatile unsigned _reserved6 : 1; // @6
		volatile enum EmptyLevel {
			Empty = 1,
			Half = 0,
		} TransferEmptyLevel : 1; // @7
		volatile enum EmptyLevel PeriodicTransferEmptyLevel : 1; // @8
		volatile unsigned _reserved9_20 : 12; // @9
		volatile bool remmemsupp:1; // @21
		volatile bool notialldmawrit:1; // @22
		volatile enum {
			Incremental = 0,
			Single = 1, // (default)
		} DmaRemainderMode : 1; // @23
		volatile unsigned _reserved24_31 : 8; // @24
	} __attribute__ ((__packed__)) Ahb;	// +0x8
	volatile struct {
		volatile unsigned toutcal:3; // @0
		volatile bool PhyInterface : 1; // @3
		volatile enum UMode {
			ULPI,
			UTMI,
		}  ModeSelect : 1; // @4
		volatile bool fsintf:1; // @5
		volatile bool physel:1; // @6
		volatile bool ddrsel:1; // @7
		volatile bool SrpCapable : 1; // @8
		volatile bool HnpCapable : 1; // @9
		volatile unsigned usbtrdtim:4; // @10
		volatile unsigned reserved1:1; // @14
		/* PHY lower power mode clock select */
		volatile bool phy_lpm_clk_sel:1; // @15
		volatile bool otgutmifssel:1; // @16
		volatile bool UlpiFsls : 1; // @17
		volatile bool ulpi_auto_res:1; // @18
		volatile bool ulpi_clk_sus_m:1; // @19
		volatile bool UlpiDriveExternalVbus : 1; // @20
		volatile bool ulpi_int_vbus_indicator:1; // @21
		volatile bool TsDlinePulseEnable : 1; // @22
		volatile bool indicator_complement:1; // @23
		volatile bool indicator_pass_through:1; // @24
		volatile bool ulpi_int_prot_dis:1; // @25
		volatile bool ic_usb_capable:1; // @26
		volatile bool ic_traffic_pull_remove:1; // @27
		volatile bool tx_end_delay:1; // @28
		volatile bool force_host_mode:1; // @29
		volatile bool force_dev_mode:1; // @30
		volatile unsigned _reserved31:1; // @31
	} __attribute__ ((__packed__)) Usb; // +0xc
	volatile struct CoreReset {
		volatile bool CoreSoft : 1; // @0
		volatile bool HclkSoft : 1; // @1
		volatile bool HostFrameCounter : 1; // @2
		volatile bool InTokenQueueFlush : 1; // @3
		volatile bool ReceiveFifoFlush : 1; // @4
		volatile bool TransmitFifoFlush : 1; // @5
		volatile enum CoreFifoFlush {
			FlushNonPeriodic = 0,
			FlushPeriodic1 = 1,
			FlushPeriodic2 = 2,
			FlushPeriodic3 = 3,
			FlushPeriodic4 = 4,
			FlushPeriodic5 = 5,
			FlushPeriodic6 = 6,
			FlushPeriodic7 = 7,
			FlushPeriodic8 = 8,
			FlushPeriodic9 = 9,
			FlushPeriodic10 = 10,
			FlushPeriodic11 = 11,
			FlushPeriodic12 = 12,
			FlushPeriodic13 = 13,
			FlushPeriodic14 = 14,
			FlushPeriodic15 = 15,
			FlushAll = 16,
		} TransmitFifoFlushNumber : 5; // @6
		volatile unsigned _reserved11_29 : 19; // @11
		volatile bool DmaRequestSignal : 1; // @30
		volatile bool AhbMasterIdle : 1; // @31
	} __attribute__ ((__packed__)) Reset;  // +0x10
	volatile struct CoreInterrupts Interrupt; // +0x14
	volatile struct CoreInterrupts InterruptMask; // +0x18
	volatile struct {
		volatile struct ReceiveStatus {
			volatile unsigned ChannelNumber : 4; // @0
			volatile unsigned bcnt : 11; // @4
			volatile unsigned dpid : 2; // @15
			volatile enum {
				InPacket = 2,
				InTransferComplete = 3,
				DataToggleError = 5,
				ChannelHalted = 7,
			} PacketStatus : 4; // @17
			volatile unsigned _reserved21_31 : 11; // @21
		} __attribute__ ((__packed__)) Peek; // Read Only +0x1c
		volatile const struct ReceiveStatus Pop; // Read Only +0x20
		volatile u32 Size; // +0x24
	} __attribute__ ((__packed__)) Receive; // +0x1c
	volatile struct {
		volatile struct FifoSize {
			volatile unsigned StartAddress : 16; // @0
			volatile unsigned Depth : 16; // @16
		} __attribute__ ((__packed__)) Size; // +0x28
		volatile const struct {
			volatile unsigned SpaceAvailable : 16; // @0
			volatile unsigned QueueSpaceAvailable : 8; // @16
			volatile unsigned Terminate : 1; // @24
			volatile enum {
				InOut = 0,
				ZeroLengthOut = 1,
				PingCompleteSplit = 2,
				ChannelHalt = 3,
			} TokenType : 2; // @25
			volatile unsigned Channel : 4; // @27
			volatile unsigned Odd : 1; // @31
		} __attribute__ ((__packed__)) Status; // Read Only +0x2c
	} __attribute__ ((__packed__)) NonPeriodicFifo; // +0x28
	volatile struct {
		unsigned ReadWriteData : 8; // @0
		unsigned RegisterAddress : 8; // @8
		unsigned Address : 7; // @16
		bool I2cEnable : 1; // @23
		bool Acknowledge : 1; // @24
		bool I2cSuspendControl : 1; // @25
		unsigned I2cDeviceAddress : 2; // @26
		unsigned _reserved28_29 : 2; // @28
		bool ReadWrite : 1; // @30
		bool bsydne : 1; // @31
	} __attribute__ ((__packed__)) I2cControl; // +0x30
	volatile u32 PhyVendorControl; // +0x34
	volatile u32 Gpio; // +0x38
	volatile u32 UserId; // +0x3c
	volatile const u32 VendorId; // Read Only +0x40
	volatile const struct {
		volatile const unsigned Direction0 : 2;
		volatile const unsigned Direction1 : 2;
		volatile const unsigned Direction2 : 2;
		volatile const unsigned Direction3 : 2;
		volatile const unsigned Direction4 : 2;
		volatile const unsigned Direction5 : 2;
		volatile const unsigned Direction6 : 2;
		volatile const unsigned Direction7 : 2;
		volatile const unsigned Direction8 : 2;
		volatile const unsigned Direction9 : 2;
		volatile const unsigned Direction10 : 2;
		volatile const unsigned Direction11 : 2;
		volatile const unsigned Direction12 : 2;
		volatile const unsigned Direction13 : 2;
		volatile const unsigned Direction14 : 2;
		volatile const unsigned Direction15 : 2;
		volatile const enum {
			HNP_SRP_CAPABLE,
			SRP_ONLY_CAPABLE,
			NO_HNP_SRP_CAPABLE,
			SRP_CAPABLE_DEVICE,
			NO_SRP_CAPABLE_DEVICE,
			SRP_CAPABLE_HOST,
			NO_SRP_CAPABLE_HOST,
		} OperatingMode : 3; // @32
		volatile const enum {
			SlaveOnly,
			ExternalDma,
			InternalDma,
		} Architecture : 2; // @35
		bool PointToPoint : 1; // @37
		volatile const enum {
			NotSupported,
			Utmi,
			Ulpi,
			UtmiUlpi,
		} HighSpeedPhysical : 2; // @38
		volatile const enum {
			Physical0,
			Dedicated,
			Physical2,
			Physcial3,
		} FullSpeedPhysical : 2; // @40
		volatile const unsigned DeviceEndPointCount : 4; // @42
		volatile const unsigned HostChannelCount : 4; // @46
		volatile const bool SupportsPeriodicEndpoints : 1; // @50
		volatile const bool DynamicFifo : 1; // @51
		volatile const bool multi_proc_int:1; // @52
		volatile const unsigned _reserver21 : 1; // @53
		volatile const unsigned NonPeriodicQueueDepth : 2; // @54
		volatile const unsigned HostPeriodicQueueDepth : 2; // @56
		volatile const unsigned DeviceTokenQueueDepth : 5; // @58
		volatile const bool EnableIcUsb : 1; // @63
		volatile const unsigned TransferSizeControlWidth : 4; // @64
		volatile const unsigned PacketSizeControlWidth : 3; // @68
		volatile const bool otg_func:1; // @71
		volatile const bool I2c : 1; // @72
		volatile const bool VendorControlInterface : 1; // @73
		volatile const bool OptionalFeatures : 1; // @74
		volatile const bool SynchronousResetType : 1; // @75
		volatile const bool AdpSupport : 1; // @76
		volatile const bool otg_enable_hsic:1; // @77
		volatile const bool bc_support:1; // @78
		volatile const bool LowPowerModeEnabled : 1; // @79
		volatile const unsigned FifoDepth : 16;  // @80
		volatile const unsigned PeriodicInEndpointCount : 4; // @96
		volatile const bool PowerOptimisation : 1; // @100
		volatile const bool MinimumAhbFrequency : 1; // @101
		volatile const bool PartialPowerOff : 1; // @102
		volatile const unsigned _reserved103_109 : 7;  // @103
		volatile const enum {
			Width8bit,
			Width16bit,
			Width8or16bit,
		} UtmiPhysicalDataWidth : 2; // @110
		volatile const unsigned ModeControlEndpointCount : 4; // @112
		volatile const bool ValidFilterIddigEnabled : 1; // @116
		volatile const bool VbusValidFilterEnabled : 1; // @117
		volatile const bool ValidFilterAEnabled : 1; // @118
		volatile const bool ValidFilterBEnabled : 1; // @119
		volatile const bool SessionEndFilterEnabled : 1; // @120
		volatile const bool ded_fifo_en:1; // @121
		volatile const unsigned InEndpointCount : 4; // @122
		volatile const bool DmaDescription : 1; // @126
		volatile const bool DmaDynamicDescription : 1; // @127
	} __attribute__ ((__packed__)) Hardware; // All read only +0x44
	volatile struct {
		volatile bool LowPowerModeCapable : 1; // @0
		volatile bool ApplicationResponse : 1; // @1
		volatile unsigned HostInitiatedResumeDuration : 4; // @2
		volatile bool RemoteWakeupEnabled : 1; // @6
		volatile bool UtmiSleepEnabled : 1; // @7
		volatile unsigned HostInitiatedResumeDurationThreshold : 5; // @8
		volatile unsigned LowPowerModeResponse : 2; // @13
		volatile bool PortSleepStatus : 1;  // @15
		volatile bool SleepStateResumeOk : 1; // @16
		volatile unsigned LowPowerModeChannelIndex : 4; // @17
		volatile unsigned RetryCount : 3; // @21
		volatile bool SendLowPowerMode : 1; // @24
		volatile unsigned RetryCountStatus : 3; // @25
		volatile unsigned _reserved28_29 : 2; // @28
		volatile bool HsicConnect : 1; // @30
		volatile bool InverseSelectHsic : 1; // @31
	} __attribute__ ((__packed__)) LowPowerModeConfiguration; // +0x54
	volatile const u8 _reserved58_80[0x80 - 0x58]; // No read or write +0x58
#ifdef BROADCOM_2835
	volatile struct {
		volatile const unsigned Read : 16; // Read Only @0
		volatile unsigned ClockRatio : 4; // @16
		volatile bool FreeRun : 1; // @20
		volatile bool BithashEnable : 1; // @21
		volatile bool MdcWrite : 1; // @22
		volatile bool MdoWrite : 1; // @23
		volatile unsigned _reserved24_30 : 7; // @24
		volatile const bool Busy : 1; // @31
	} __attribute__ ((__packed__)) MdioControl; // +0x80
	volatile union {
		volatile const u32 MdioRead; // Read Only +0x84
		volatile u32 MdioWrite; // +0x84
	};
	volatile struct {
		volatile bool SessionEnd : 1; // @0
		volatile bool VbusValid : 1; // @1
		volatile bool BSessionValid : 1; // @2
		volatile bool ASessionValid : 1; // @3
		volatile const bool DischargeVbus : 1; // Read Only @4
		volatile const bool ChargeVbus : 1; // Read Only @5
		volatile const bool DriveVbus : 1; // Read Only @6
		volatile bool DisableDriving : 1; // @7
		volatile bool VbusIrqEnabled : 1; // @8
		volatile const bool VbusIrq : 1; // Cleared on Read! @9
		volatile unsigned _reserved10_15 : 6; // @10
		volatile unsigned AxiPriorityLevel : 4; // @16
		volatile unsigned _reserved20_31 : 12; // @20
	} __attribute__ ((__packed__)) MiscControl; // +0x88
#else
	volatile u32 _reserved80_8c[3]; // +0x80
#endif
	volatile u8 _reserved8c_100[0x100-0x8c]; // +0x8c
	volatile struct {
		volatile struct FifoSize HostSize; // +0x100
		volatile struct FifoSize DataSize[15]; // +0x104
	} __attribute__ ((__packed__)) PeriodicFifo; // +0x100
	volatile u8 _reserved140_400[0x400-0x140]; // +0x140
};

/**
	\brief Contains the host mode global registers structure that control the HCD.

	Contains the host mode global registers structure that controls the DesignWare®
	Hi-Speed USB 2.0 On-The-Go (HS OTG) Controller.
*/
__attribute__ ((__packed__))
struct HostGlobalRegs {
	volatile struct {
		volatile enum {
			Clock30_60MHz,
			Clock48MHz,
			Clock6MHz
		} ClockRate : 2; // @0
		volatile bool FslsOnly : 1; // @2
		volatile unsigned _reserved3_6 : 4; // @3
		volatile unsigned en_32khz_susp:1; // @7
		volatile unsigned res_val_period:8; // @8
		volatile unsigned _reserved16_22 : 7; // @16
		volatile bool EnableDmaDescriptor : 1; // @23
		volatile unsigned FrameListEntries : 2; // @24
		volatile bool PeriodicScheduleEnable : 1; // @26
		volatile const bool PeriodicScheduleStatus : 1; // @27
		volatile unsigned reserved28_30 : 3; // @28
		volatile bool mode_chg_time:1; // @31
	} __attribute__ ((__packed__)) Config; // +0x400
	volatile struct {
		volatile unsigned Interval : 16; // @0
		volatile bool DynamicFrameReload : 1; // @16
		volatile unsigned _reserved17_31 : 15; // @17
	} __attribute__ ((__packed__)) FrameInterval; // +0x404
	volatile struct {
		volatile unsigned FrameNumber : 16; // @0
		volatile unsigned FrameRemaining : 16; // @16
	} __attribute__ ((__packed__)) FrameNumber; // +0x408
	volatile u32 _reserved40c; // + 0x40c
	volatile struct {
		volatile unsigned SpaceAvailable : 16; // @0
		volatile unsigned QueueSpaceAvailable : 8; // @16
		volatile unsigned Terminate : 1; // @24
		volatile enum {
			ZeroLength = 0,
			Ping = 1,
			Disable = 2,
		} TokenType : 2; // @25
		volatile unsigned Channel : 4; // @27
		volatile unsigned Odd : 1; // @31
	} __attribute__ ((__packed__)) FifoStatus; // +0x410
	volatile u32 Interrupt; // +0x414
	volatile u32 InterruptMask; // +0x418
	volatile u32 FrameList; // +0x41c
	volatile u8 _reserved420_440[0x440-0x420]; // +0x420
	volatile struct HostPort {
		volatile bool Connect : 1; // @0
		volatile bool ConnectDetected : 1; // @1
		volatile bool Enable : 1; // @2
		volatile bool EnableChanged : 1; // @3
		volatile bool OverCurrent : 1; // @4
		volatile bool OverCurrentChanged : 1; // @5
		volatile bool Resume : 1; // @6
		volatile bool Suspend : 1; // @7
		volatile bool Reset : 1; // @8
		volatile unsigned _reserved9 : 1; // @9
		volatile unsigned PortLineStatus : 2; // @10
		volatile bool Power : 1; // @12
		volatile unsigned TestControl : 4; // @13
		volatile UsbSpeed Speed : 2; // @17
		volatile unsigned _reserved19_31 : 13; // @19
	} __attribute__ ((__packed__)) Port; // +0x440
	volatile u8 _reserved444_500[0x500 - 0x444]; // +0x444
	volatile struct HostChannel {
		volatile struct HostChannelCharacteristic {
			volatile unsigned MaximumPacketSize : 11; // @0
			volatile unsigned EndPointNumber : 4; // @11
			volatile UsbDirection EndPointDirection : 1; // @15
			volatile unsigned _reserved16 : 1; // @16
			volatile bool LowSpeed : 1; // @17
			UsbTransfer Type : 2; // @18
			volatile unsigned PacketsPerFrame : 2; // @20
			volatile unsigned DeviceAddress : 7; // @22
			volatile unsigned OddFrame  : 1; // @29
			volatile bool Disable : 1; // @30
			volatile bool Enable : 1; // @31
		} __attribute__ ((__packed__)) Characteristic; // +0x0
		volatile struct {
			volatile unsigned PortAddress : 7; // @0
			volatile unsigned HubAddress : 7; // @7
			volatile enum {
				Middle = 0,
				End = 1,
				Begin = 2,
				All = 3,
			} TransactionPosition : 2; // @14
			volatile bool CompleteSplit : 1; // @16
			volatile unsigned _reserved17_30 : 14; // @17
			volatile bool SplitEnable : 1; // @31
		} __attribute__ ((__packed__)) SplitControl; // +0x4
		volatile struct ChannelInterrupts Interrupt; // +0x8
		volatile struct ChannelInterrupts InterruptMask; // +0xc
		volatile struct {
			volatile unsigned TransferSize : 19; // @0
			volatile unsigned PacketCount : 10; // @19
			volatile enum PacketId {
				Data0 = 0,
				Data1 = 2,
				Data2 = 1,
				MData = 3,
				Setup = 3,
			} PacketId : 2; // @29
			volatile bool DoPing : 1; // @31
		} __attribute__ ((__packed__)) TransferSize; // +0x10
		volatile void* DmaAddress;  // +0x14
		volatile u32 _reserved18; // +0x18
		volatile u32 _reserved1c; // +0x1c
	} __attribute__ ((__packed__)) Channel[ChannelCount]; // +0x500
	volatile u8 _reserved700_800[0x800 - 0x700]; // +0x700
};

/**
	\brief Contains the dwc power and clock gating controls.

	Contains the dwc power and clock gating structure that controls the DesignWare®
	Hi-Speed USB 2.0 On-The-Go (HS OTG) Controller.
*/
__attribute__ ((__packed__))
struct PowerReg {
	volatile bool StopPClock : 1; // @0
	volatile bool GateHClock : 1; // @1
	volatile bool PowerClamp : 1; // @2
	volatile bool PowerDownModules : 1; // @3
	volatile bool PhySuspended : 1; // @4
	volatile bool EnableSleepClockGating : 1; // @5
	volatile bool PhySleeping : 1; // @6
	volatile bool DeepSleep : 1; // @7
	volatile unsigned _reserved8_31 : 24; // @8
};

/** 
	\brief Indicates if the Phy has been initialised.

	Indicaes if the Phy has been initialised, as this can only be done once.
*/

/**	
	\brief The device number of the root hub.

	We keep track of the device number of the virtual root hub we are 
	simulating.
*/

/**
	\brief Sends a message to the virtual root hub for processing.

	Passes a message to the virtual root hub for processing. The buffer can be 
	both in and out. 
*/
static Result HcdProcessRootHubMessage(struct UsbDevice *device, struct 
    UsbPipeAddress pipe, void* buffer, u32 bufferLength,
	struct UsbDeviceRequest *request);

/** 
	\brief Writes through a register from the cache to the device.

	Causes a register in the cache to be written through to the host device. To 
	avoid unintended consequences, this method prevents modifications to 
	certain registers, or fields within registers, such as those whith 
	differing read/write behaviour.
*/
static void WriteThroughReg(volatile const void* reg);
/** 
	\brief Writes through a register from the cache to the device.

	Causes a register in the cache to be written through to the host device. To 
	avoid unintended consequences, this method prevents modifications to 
	certain registers, or fields within registers, such as those whith 
	differing read/write behaviour. The mask is ored with the standard mask for
	different read write behaviour so that these can be set.
*/
static void WriteThroughRegMask(volatile const void* reg, u32 maskOr);
/** 
	\brief Reads back a register from the device to the cache.

	Causes a register on the host device to be copied back to the cache. To 
	avoid unintended consequences, this method prevents reading to 
	certain registers, or fields within registers, such as those whith 
	differing read/write behaviour. Specific methods are provided in place of
	this for such functions.
*/
static void ReadBackReg(volatile const void* reg);
/** 
	\brief Resets a register within the cache.

	Causes a register in the cache to be reset to it's default values. Exact 
	meanings differ, but it is generally in preparation to write back a fresh 
	value to a field.
*/
static void ClearReg(volatile const void* reg);
/** 
	\brief Resets a register within the cache to all on.

	Causes a register in the cache to be reset to it's 'on' values. Exact 
	meanings differ, but it is generally in preparation to write back a 
	field mostly to 1. Used in interrutps for example to set all the 
	interrutps to 1 in preparation for reseting the interrupt flags.
*/
static void SetReg(volatile const void* reg);

#endif // HCD_DESIGNWARE_20

#ifdef __cplusplus
}
#endif

#endif // _HCD_DWC_DESIGNWARE_H
/******************************************************************************
*	platform/none/byteorder.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	platform/none/byteorder.h contains generic definitions for changing the 
*	byte order of halfs to match the processor.
******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

#if defined ENDIAN_BIG
/**
	Converts a number from the machine's byte order to big endian or back.
*/
#define EndianBigWord(x) x
/**
	Converts a number from the machine's byte order to little endian or back.
*/
#define EndianLittleWord(x) ({ u32 t = x; (t >> 24) & 0xff | (t >> 8) & 0xff00 | (t << 8) & 0xff00 | (t << 24) & 0xff000000; })
/**
	Converts a number from the machine's byte order to big endian or back.
*/
#define EndianBigHalf(x) x
/**
	Converts a number from the machine's byte order to little endian or back.
*/
#define EndianLittleHalf(x) ({ u16 t = x; (t >> 8) & 0xff | (t << 8) & 0xff00; })
#elif defined ENDIAN_LITTLE
/**
	Converts a number from the machine's byte order to big endian or back.
*/
#define EndianBigWord(x) ({ u32 t = x; (t >> 24) & 0xff | (t >> 8) & 0xff00 | (t << 8) & 0xff0000 | (t << 24) & 0xff000000; })
/**
	Converts a number from the machine's byte order to little endian or back.
*/
#define EndianLittleWord(x) x
/**
	Converts a number from the machine's byte order to big endian or back.
*/
#define EndianBigHalf(x) ({ u16 t = x; (t >> 8) & 0xff | (t << 8) & 0xff00; })
/**
	Converts a number from the machine's byte order to little endian or back.
*/
#define EndianLittleHalf(x) x
#else
#error Endianness not specified.
#endif

#ifdef __cplusplus
}
#endif
/******************************************************************************
*	platform/arm/armv6.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	platform/arm/armv6.h contains definitions for arm version 6 processors.
******************************************************************************/
/******************************************************************************
*	platform/arm/broadcom2835.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	platform/arm/broadcom2835.h contains definitions pertaining to the
*	broadcom2835 chip, used in the Raspberry Pi.
******************************************************************************/


#ifdef __cplusplus
extern "C"
{
#endif

/** The null address. */
#define NULL ((void*)0)
#ifdef MEM_INTERNAL_MANAGER 
// When asked to use internal memory management, we use the default.
#define MEM_INTERNAL_MANAGER_DEFAULT
#endif

#ifdef __cplusplus
}
#endif
/******************************************************************************
*	configuration.c
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	configuration.c contains code to load all components. In order to
*	allow the actual source files to be completely independent, one file must
*	exist which depends upon all of them, to perform static initialisation.
*	Each separate 'library' provides a Load method, which ConfigurationLoad
*	simply invoeks all of.
******************************************************************************/


// Add load methods for new modules wrapped in ifdefs here:
static void UsbLoad();
static void PlatformLoad();
#ifdef LIB_ARM_V6
static void Arm6Load();
#endif
#ifdef LIB_BCM2835
static void Bcm2835Load();
#endif
#ifdef LIB_DWC
static void DwcLoad();
#endif
#ifdef LIB_HID
static void HidLoad();
#endif
#ifdef LIB_HUB
static void HubLoad();
#endif
#ifdef LIB_KBD
static void KbdLoad();
#endif
#ifdef LIB_MOUSE
static void MouseLoad();
#endif

static void ConfigurationLoad() {
	// Call each libraries load method here. Watch out for the order, load more
	// generic libraries first.
	PlatformLoad();
#ifdef LIB_ARM_V6
	Arm6Load();
#endif
#ifdef LIB_BCM2835
	Bcm2835Load();
#endif
	UsbLoad();
#ifdef LIB_DWC
	DwcLoad();
#endif
#ifdef LIB_HID
	HidLoad();
#endif
#ifdef LIB_HUB
	HubLoad();
#endif
#ifdef LIB_KBD
	KbdLoad();
#endif
#ifdef LIB_MOUSE
	MouseLoad();
#endif
}
/******************************************************************************
*	device/hub.c
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	device/hub.c contains code relating to the generic USB driver's hubs. USB
*	is designed such that this driver's interface would be virtually the same
*	across all systems, and in fact its implementation varies little either.
******************************************************************************/

#define ControlMessageTimeout 10

#define HidMessageTimeout 10
static void ConfigurationLoad();


static Result (*HidUsageAttach[HidUsageAttachCount])(struct UsbDevice *device, 
u32 interfaceNumber) = {0,0,0,0,0,0,0,0,0,0};

/** The default timeout in ms of control transfers. */
#define ControlMessageTimeout 10
/** The maximum number of devices that can be connected. */
#define MaximumDevices 32

static struct UsbDevice *Devices[MaximumDevices] = {0};
static Result (*InterfaceClassAttach[InterfaceClassAttachCount])(struct UsbDevice *device, u32 interfaceNumber) = {0};
static u32 RootHubDeviceNumber = 0;

static volatile struct CoreGlobalRegs *CorePhysical=NULL, *Core = NULL;
 
static volatile struct HostGlobalRegs *HostPhysical=NULL, *Host = NULL;
 
static volatile struct PowerReg *PowerPhysical=NULL, *Power = NULL;
 
static bool PhyInitialised = false;
static u8* databuffer = NULL;

static void HubLoad() {
	LOG_DEBUG("CSUD: Hub driver version 0.1\n"); 
	InterfaceClassAttach[InterfaceClassHub] = HubAttach;
}

static Result HubReadDescriptor(struct UsbDevice *device) {
	struct UsbDescriptorHeader header;
	Result result;

	if ((result = UsbGetDescriptor(device, Hub, 0, 0, &header, sizeof(header), sizeof(header), 0x20)) != OK) {
		LOGF("HUB: Failed to read hub descriptor for %s.\n", UsbGetDescription(device));
		return result;
	}
	if (((struct HubDevice*)device->DriverData)->Descriptor == NULL &&
		(((struct HubDevice*)device->DriverData)->Descriptor = MemoryAllocate(header.DescriptorLength)) == NULL) {
		LOGF("HUB: Not enough memory to read hub descriptor for %s.\n", UsbGetDescription(device));
		return ErrorMemory;
	}
	if ((result = UsbGetDescriptor(device, Hub, 0, 0, ((struct HubDevice*)device->DriverData)->Descriptor, header.DescriptorLength, header.DescriptorLength, 0x20)) != OK) {
		LOGF("HUB: Failed to read hub descriptor for %s.\n", UsbGetDescription(device));
		return result;
	}

	return OK;
}

static Result HubGetStatus(struct UsbDevice *device) {
	Result result;
	
	if ((result = UsbControlMessage(
		device, 
		(struct UsbPipeAddress) { 
			.Type = Control, 
			.Speed = device->Speed, 
			.EndPoint = 0 , 
			.Device = device->Number, 
			.Direction = In,
			.MaxSize = SizeFromNumber(device->Descriptor.MaxPacketSize0),
		},
		&((struct HubDevice*)device->DriverData)->Status,
		sizeof(struct HubFullStatus),
		&(struct UsbDeviceRequest) {
			.Request = GetStatus,
			.Type = 0xa0,
			.Length = sizeof(struct HubFullStatus),
		},
		ControlMessageTimeout)) != OK) 
		return result;
	if (device->LastTransfer < sizeof(struct HubFullStatus)) {
		LOGF("HUB: Failed to read hub status for %s.\n", UsbGetDescription(device));
		return ErrorDevice;
	}
	return OK;
}

static Result HubPortGetStatus(struct UsbDevice *device, u8 port) {
	Result result;
	
	if ((result = UsbControlMessage(
		device, 
		(struct UsbPipeAddress) { 
			.Type = Control, 
			.Speed = device->Speed, 
			.EndPoint = 0, 
			.Device = device->Number, 
			.Direction = In,
			.MaxSize = SizeFromNumber(device->Descriptor.MaxPacketSize0),
		},
		&((struct HubDevice*)device->DriverData)->PortStatus[port],
		sizeof(struct HubPortFullStatus),
		&(struct UsbDeviceRequest) {
			.Request = GetStatus,
			.Type = 0xa3,
			.Index = port + 1,
			.Length = sizeof(struct HubPortFullStatus),
		},
		ControlMessageTimeout)) != OK) 
		return result;
	if (device->LastTransfer < sizeof(struct HubPortFullStatus)) {
		LOGF("HUB: Failed to read hub port status for %s.Port%d.\n", UsbGetDescription(device), port + 1);
		return ErrorDevice;
	}
	return OK;
}

static Result HubChangeFeature(struct UsbDevice *device, enum HubFeature 
    feature, bool set) {
	Result result;
	
	if ((result = UsbControlMessage(
		device, 
		(struct UsbPipeAddress) { 
			.Type = Control, 
			.Speed = device->Speed, 
			.EndPoint = 0, 
			.Device = device->Number, 
			.Direction = Out,
			.MaxSize = SizeFromNumber(device->Descriptor.MaxPacketSize0),
		},
		NULL,
		0,
		&(struct UsbDeviceRequest) {
			.Request = set ? SetFeature : ClearFeature,
			.Type = 0x20,
			.Value = (u8)feature,
		},
		ControlMessageTimeout)) != OK) 
		return result;

	return OK;
}

Result HubChangePortFeature(struct UsbDevice *device, enum HubPortFeature feature, u8 port, bool set) {
	Result result;
	
	if ((result = UsbControlMessage(
		device, 
		(struct UsbPipeAddress) { 
			.Type = Control, 
			.Speed = device->Speed, 
			.EndPoint = 0, 
			.Device = device->Number, 
			.Direction = Out,
			.MaxSize = SizeFromNumber(device->Descriptor.MaxPacketSize0),
		},
		NULL,
		0,
		&(struct UsbDeviceRequest) {
			.Request = set ? SetFeature : ClearFeature,
			.Type = 0x23,
			.Value = (u16)feature,
			.Index = port + 1,			
		},
		ControlMessageTimeout)) != OK) 
		return result;

	return OK;
}

Result HubPowerOn(struct UsbDevice *device) {
	struct HubDevice *data;
	struct HubDescriptor *hubDescriptor;
	
	data = (struct HubDevice*)device->DriverData;
	hubDescriptor = data->Descriptor;
	LOG_DEBUGF("HUB: Powering on hub %s.\n", UsbGetDescription(device));
	
	for (u32 i = 0; i < data->MaxChildren; i++) {
		if (HubChangePortFeature(device, FeaturePower, i, true) != OK)
			LOGF("HUB: Could not power %s.Port%d.\n", UsbGetDescription(device), i + 1);
	}

	MicroDelay(hubDescriptor->PowerGoodDelay * 2000);

	return OK;
}

Result HubPortReset(struct UsbDevice *device, u8 port) {
	Result result;
	struct HubDevice *data;
	struct HubPortFullStatus *portStatus;
	u32 retry, timeout;

	data = (struct HubDevice*)device->DriverData;
	portStatus = &data->PortStatus[port];

	LOG_DEBUGF("HUB: Hub reset %s.Port%d.\n", UsbGetDescription(device), port + 1);
	for (retry = 0; retry < 3; retry++) {
		if ((result = HubChangePortFeature(device, FeatureReset, port, true)) != OK) {
			LOGF("HUB: Failed to reset %s.Port%d.\n", UsbGetDescription(device), port + 1);
			return result;
		}
		timeout = 0;
		do {
			MicroDelay(20000);
			if ((result = HubPortGetStatus(device, port)) != OK) {
				LOGF("HUB: Hub failed to get status (4) for %s.Port%d.\n", UsbGetDescription(device), port + 1);
				return result;
			}			
			timeout++;
		} while (!portStatus->Change.ResetChanged && !portStatus->Status.Enabled && timeout < 10);

		if (timeout == 10)
			continue;
		
		LOG_DEBUGF("HUB: %s.Port%d Status %x:%x.\n", UsbGetDescription(device), port + 1, *(u16*)&portStatus->Status, *(u16*)&portStatus->Change);

		if (portStatus->Change.ConnectedChanged || !portStatus->Status.Connected)
			return ErrorDevice;

		if (portStatus->Status.Enabled) 
			break;
	}

	if (retry == 3) {
		LOGF("HUB: Cannot enable %s.Port%d. Please verify the hardware is working.\n", UsbGetDescription(device), port + 1);
		return ErrorDevice;
	}

	if ((result = HubChangePortFeature(device, FeatureResetChange, port, false)) != OK) {
		LOGF("HUB: Failed to clear reset on %s.Port%d.\n", UsbGetDescription(device), port + 1);
	}
	return OK;
}

Result HubPortConnectionChanged(struct UsbDevice *device, u8 port) {
	Result result;
	struct HubDevice *data;
	struct HubPortFullStatus *portStatus;

	data = (struct HubDevice*)device->DriverData;
	portStatus = &data->PortStatus[port];
	
	if ((result = HubPortGetStatus(device, port)) != OK) {
		LOGF("HUB: Hub failed to get status (2) for %s.Port%d.\n", UsbGetDescription(device), port + 1);
		return result;
	}
	LOG_DEBUGF("HUB: %s.Port%d Status %x:%x.\n", UsbGetDescription(device), port + 1, *(u16*)&portStatus->Status, *(u16*)&portStatus->Change);
	
	if ((result = HubChangePortFeature(device, FeatureConnectionChange, port, false)) != OK) {
		LOGF("HUB: Failed to clear change on %s.Port%d.\n", UsbGetDescription(device), port + 1);
	}
	
	if ((!portStatus->Status.Connected && !portStatus->Status.Enabled) || data->Children[port] != NULL) {
		LOGF("HUB: Disconnected %s.Port%d - %s.\n", UsbGetDescription(device), port + 1, UsbGetDescription(data->Children[port]));
		UsbDeallocateDevice(data->Children[port]);
		data->Children[port] = NULL;
		if (!portStatus->Status.Connected) return OK;
	}
	
	if ((result = HubPortReset(device, port)) != OK) {
		LOGF("HUB: Could not reset %s.Port%d for new device.\n", UsbGetDescription(device), port + 1);
		return result;
	}

	if ((result = UsbAllocateDevice(&data->Children[port])) != OK) {
		LOGF("HUB: Could not allocate a new device entry for %s.Port%d.\n", UsbGetDescription(device), port + 1);
		return result;
	}
	
	if ((result = HubPortGetStatus(device, port)) != OK) {
		LOGF("HUB: Hub failed to get status (3) for %s.Port%d.\n", UsbGetDescription(device), port + 1);
		return result;
	}
	
	LOG_DEBUGF("HUB: %s.Port%d Status %x:%x.\n", UsbGetDescription(device), port + 1, *(u16*)&portStatus->Status, *(u16*)&portStatus->Change);

	if (portStatus->Status.HighSpeedAttatched) data->Children[port]->Speed = High;
	else if (portStatus->Status.LowSpeedAttatched) data->Children[port]->Speed = Low;
	else data->Children[port]->Speed = Full;
	data->Children[port]->Parent = device;
	data->Children[port]->PortNumber = port;
	if ((result = UsbAttachDevice(data->Children[port])) != OK) {
		LOGF("HUB: Could not connect to new device in %s.Port%d. Disabling.\n", UsbGetDescription(device), port + 1);
		UsbDeallocateDevice(data->Children[port]);
		data->Children[port] = NULL;
		if (HubChangePortFeature(device, FeatureEnable, port, false) != OK) {
			LOGF("HUB: Failed to disable %s.Port%d.\n", UsbGetDescription(device), port + 1);
		}
		return result;
	}
	return OK;
}

static void HubDetached(struct UsbDevice *device) {
	struct HubDevice *data;

	LOG_DEBUGF("HUB: %s detached.\n", UsbGetDescription(device));
	if (device->DriverData != NULL) {
		data = (struct HubDevice*)device->DriverData;
		
		for (u32 i = 0; i < data->MaxChildren; i++) {
			if (data->Children[i] != NULL &&
				data->Children[i]->DeviceDetached != NULL)
				data->Children[i]->DeviceDetached(data->Children[i]);
		}
	}
}

static void HubDeallocate(struct UsbDevice *device) {
	struct HubDevice *data;
	
	LOG_DEBUGF("HUB: %s deallocate.\n", UsbGetDescription(device));
	if (device->DriverData != NULL) {
		data = (struct HubDevice*)device->DriverData;
		
		for (u32 i = 0; i < data->MaxChildren; i++) {
			if (data->Children[i] != NULL) {
				UsbDeallocateDevice(data->Children[i]);
				data->Children[i] = NULL;
			}
		}
			
		if (data->Descriptor != NULL)
			MemoryDeallocate(data->Descriptor);
		MemoryDeallocate((void*)device->DriverData);
	}
	device->DeviceDeallocate = NULL;
	device->DeviceDetached = NULL;
	device->DeviceCheckForChange = NULL;
	device->DeviceChildDetached = NULL;
	device->DeviceChildReset = NULL;
	device->DeviceCheckConnection = NULL;
}

static void HubCheckForChange(struct UsbDevice *device) {
	struct HubDevice *data;
	
	data = (struct HubDevice*)device->DriverData;
	
	for (u32 i = 0; i < data->MaxChildren; i++) {
		if (HubCheckConnection(device, i) != OK)
			continue;

		if (data->Children[i] != NULL && 
			data->Children[i]->DeviceCheckForChange != NULL)
				data->Children[i]->DeviceCheckForChange(data->Children[i]);
	}
}

static void HubChildDetached(struct UsbDevice *device, struct UsbDevice *child) 
    {
	struct HubDevice *data;
	
	data = (struct HubDevice*)device->DriverData;
	
	if (child->Parent == device && child->PortNumber >= 0 && child->PortNumber < data->MaxChildren &&
		data->Children[child->PortNumber] == child)
		data->Children[child->PortNumber] = NULL;
}

static Result HubChildReset(struct UsbDevice *device, struct UsbDevice *child) 
    {
	struct HubDevice *data;
	
	data = (struct HubDevice*)device->DriverData;
	
	if (child->Parent == device && child->PortNumber >= 0 && child->PortNumber < data->MaxChildren &&
		data->Children[child->PortNumber] == child)
		return HubPortReset(device, child->PortNumber);
	else
		return ErrorDevice;
}

static Result HubCheckConnectionDevice(struct UsbDevice *device, struct 
    UsbDevice *child) {
	struct HubDevice *data;
	Result result;

	data = (struct HubDevice*)device->DriverData;
	
	if (child->Parent == device && child->PortNumber >= 0 && child->PortNumber < data->MaxChildren &&
		data->Children[child->PortNumber] == child) {
		if ((result = HubCheckConnection(device, child->PortNumber)) != OK)
			return result;
		return data->Children[child->PortNumber] == child ? OK : ErrorDisconnected;
	}
	else
		return ErrorArgument;
}

static Result HubCheckConnection(struct UsbDevice *device, u8 port) {
	Result result;
	struct HubPortFullStatus *portStatus;
	struct HubDevice *data;

	data = (struct HubDevice*)device->DriverData;

	if ((result = HubPortGetStatus(device, port)) != OK) {
		if (result != ErrorDisconnected)
			LOGF("HUB: Failed to get hub port status (1) for %s.Port%d.\n", UsbGetDescription(device), port + 1);
		return result;
	}
	portStatus = &data->PortStatus[port];
	
	if (portStatus->Change.ConnectedChanged) {
		HubPortConnectionChanged(device, port);
	}
	if (portStatus->Change.EnabledChanged) {
		if (HubChangePortFeature(device, FeatureEnableChange, port, false) != OK) {
			LOGF("HUB: Failed to clear enable change %s.Port%d.\n", UsbGetDescription(device), port + 1);
		}
	
		// This may indicate EM interference.
		if (!portStatus->Status.Enabled && portStatus->Status.Connected && data->Children[port] != NULL) {
			LOGF("HUB: %s.Port%d has been disabled, but is connected. This can be cause by interference. Reenabling!\n", UsbGetDescription(device), port + 1);
			HubPortConnectionChanged(device, port);
		}
	}
	if (portStatus->Status.Suspended) {			
		if (HubChangePortFeature(device, FeatureSuspend, port, false) != OK) {
			LOGF("HUB: Failed to clear suspended port - %s.Port%d.\n", UsbGetDescription(device), port + 1);
		}
	}
	if (portStatus->Change.OverCurrentChanged) {		
		if (HubChangePortFeature(device, FeatureOverCurrentChange, port, false) != OK) {
			LOGF("HUB: Failed to clear over current port - %s.Port%d.\n", UsbGetDescription(device), port + 1);
		}
		HubPowerOn(device);
	}
	if (portStatus->Change.ResetChanged) {
		if (HubChangePortFeature(device, FeatureResetChange, port, false) != OK) {
			LOGF("HUB: Failed to clear reset port - %s.Port%d.\n", UsbGetDescription(device), port + 1);
		}
	}

	return OK;
}

static Result HubAttach(struct UsbDevice *device, u32 interfaceNumber) {
	Result result;
	struct HubDevice *data;
	struct HubDescriptor *hubDescriptor;
	struct HubFullStatus *status;
	
	if (device->Interfaces[interfaceNumber].EndpointCount != 1) {
		LOGF("HUB: Cannot enumerate hub with multiple endpoints: %d.\n", device->Interfaces[interfaceNumber].EndpointCount);
		return ErrorIncompatible;
	}
	if (device->Endpoints[interfaceNumber][0].EndpointAddress.Direction == Out) {
		LOG("HUB: Cannot enumerate hub with only one output endpoint.\n");
		return ErrorIncompatible;
	}
	if (device->Endpoints[interfaceNumber][0].Attributes.Type != Interrupt) {
		LOG("HUB: Cannot enumerate hub without interrupt endpoint.\n");
		return ErrorIncompatible;
	}

	device->DeviceDeallocate = HubDeallocate;
	device->DeviceDetached = HubDetached;
	device->DeviceCheckForChange = HubCheckForChange;
	device->DeviceChildDetached = HubChildDetached;
	device->DeviceChildReset = HubChildReset;
	device->DeviceCheckConnection = HubCheckConnectionDevice;
	if ((device->DriverData = MemoryAllocate(sizeof(struct HubDevice))) == NULL) {
		LOG("HUB: Cannot allocate hub data. Out of memory.\n");
		return ErrorMemory;
	}
	data = (struct HubDevice*)device->DriverData;
	device->DriverData->DataSize = sizeof(struct HubDevice);
	device->DriverData->DeviceDriver = DeviceDriverHub;
	for (u32 i = 0; i < MaxChildrenPerDevice; i++)
		data->Children[i] = NULL;

	if ((result = HubReadDescriptor(device)) != OK) return result;

	hubDescriptor = data->Descriptor;
	if (hubDescriptor->PortCount > MaxChildrenPerDevice) {
		LOGF("HUB: Hub %s is too big for this driver to handle. Only the first %d ports will be used. Change MaxChildrenPerDevice in usbd/device.h.\n", UsbGetDescription(device), MaxChildrenPerDevice);
		data->MaxChildren = MaxChildrenPerDevice;
	} else
		data->MaxChildren = hubDescriptor->PortCount;

	switch (hubDescriptor->Attributes.PowerSwitchingMode) {
	case Global:
		LOG_DEBUG("HUB: Hub power: Global.\n");
		break;
	case Individual:
		LOG_DEBUG("HUB: Hub power: Individual.\n");
		break;
	default:
		LOGF("HUB: Unknown hub power type %d on %s. Driver incompatible.\n", hubDescriptor->Attributes.PowerSwitchingMode, UsbGetDescription(device));
		HubDeallocate(device);
		return ErrorIncompatible;
	}
	
	if (hubDescriptor->Attributes.Compound)
		LOG_DEBUG("HUB: Hub nature: Compound.\n");
	else
		LOG_DEBUG("HUB: Hub nature: Standalone.\n");

	switch (hubDescriptor->Attributes.OverCurrentProtection) {
	case Global:
		LOG_DEBUG("HUB: Hub over current protection: Global.\n");
		break;
	case Individual:
		LOG_DEBUG("HUB: Hub over current protection: Individual.\n");
		break;
	default:
		LOGF("HUB: Unknown hub over current type %d on %s. Driver incompatible.\n", hubDescriptor->Attributes.OverCurrentProtection, UsbGetDescription(device));
		HubDeallocate(device);
		return ErrorIncompatible;
	}

	LOG_DEBUGF("HUB: Hub power to good: %dms.\n", hubDescriptor->PowerGoodDelay * 2);
	LOG_DEBUGF("HUB: Hub current required: %dmA.\n", hubDescriptor->MaximumHubPower * 2);
	LOG_DEBUGF("HUB: Hub ports: %d.\n", hubDescriptor->PortCount);
#if DEBUG
	for (u32 i = 0; i < data->MaxChildren; i++) {
		if (hubDescriptor->Data[(i + 1) >> 3] & 1 << ((i + 1) & 0x7)) 
			LOG_DEBUGF("HUB: Hub port %d is not removable.\n", i + 1);
		else
			LOG_DEBUGF("HUB: Hub port %d is removable.\n", i + 1);
	}
#endif
	
	if ((result = HubGetStatus(device)) != OK) {
		LOGF("HUB: Failed to get hub status for %s.\n", UsbGetDescription(device));
		return result;
	}
	status = &data->Status;

	if (!status->Status.LocalPower) LOG_DEBUG("USB Hub power: Good.\n");
	else LOG_DEBUG("HUB: Hub power: Lost.\n");
	if (!status->Status.OverCurrent) LOG_DEBUG("USB Hub over current condition: No.\n");
	else LOG_DEBUG("HUB: Hub over current condition: Yes.\n");

	LOG_DEBUG("HUB: Hub powering on.\n");
	if ((result = HubPowerOn(device)) != OK) {
		LOG_DEBUG("HUB: Hub failed to power on.\n");
		HubDeallocate(device);
		return result;
	}

	if ((result = HubGetStatus(device)) != OK) {
		LOGF("HUB: Failed to get hub status for %s.\n", UsbGetDescription(device));
		HubDeallocate(device);
		return result;
	}

	if (!status->Status.LocalPower) LOG_DEBUG("USB Hub power: Good.\n");
	else LOG_DEBUG("HUB: Hub power: Lost.\n");
	if (!status->Status.OverCurrent) LOG_DEBUG("USB Hub over current condition: No.\n");
	else LOG_DEBUG("HUB: Hub over current condition: Yes.\n");

	LOG_DEBUGF("HUB: %s status %x:%x.\n", UsbGetDescription(device), *(u16*)&status->Status, *(u16*)&status->Change);
	
	for (u8 port = 0; port < data->MaxChildren; port++) {
		HubCheckConnection(device, port);
	}

	return OK;
}
/******************************************************************************
*	device/hid/hid.c
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	device/hid/hid.c contains code relating to the generic USB human interface 
*	device driver. Human interface devices have another standard on top of USB
*	(oh boy!) which is actually very neat. It allows human interface devices to
*	describe their buttons, sliders and dials in great detail, and allows a 
*	flexible driver to handle them all. This driver merely provides methods to
*	deal with these reports. More abstracted drivers for keyboards and mice and
*	whatnot would no doubt be very useful.
******************************************************************************/

static void HidLoad() {
	LOG_DEBUG("CSUD: HID driver version 0.1\n"); 
	InterfaceClassAttach[InterfaceClassHid] = HidAttach;
}

static Result HidGetReport(struct UsbDevice *device, enum HidReportType 
    reportType, u8 reportId, u8 interface, u32 bufferLength, void* buffer) {
	Result result;
	
	if ((result = UsbControlMessage(
		device, 
		(struct UsbPipeAddress) { 
			.Type = Control, 
			.Speed = device->Speed, 
			.EndPoint = 0 , 
			.Device = device->Number, 
			.Direction = In,
			.MaxSize = SizeFromNumber(device->Descriptor.MaxPacketSize0),
		},
		buffer,
		bufferLength,
		&(struct UsbDeviceRequest) {
			.Request = GetReport,
			.Type = 0xa1,
			.Index = interface,
			.Value = (u16)reportType << 8 | reportId,
			.Length = bufferLength,
		},
		HidMessageTimeout)) != OK) 
		return result;

	return OK;
}

static Result HidSetReport(struct UsbDevice *device, enum HidReportType 
    reportType, u8 reportId, u8 interface, u32 bufferLength, void* buffer) {
	Result result;
	
	if ((result = UsbControlMessage(
		device, 
		(struct UsbPipeAddress) { 
			.Type = Control, 
			.Speed = device->Speed, 
			.EndPoint = 0 , 
			.Device = device->Number, 
			.Direction = Out,
			.MaxSize = SizeFromNumber(device->Descriptor.MaxPacketSize0),
		},
		buffer,
		bufferLength,
		&(struct UsbDeviceRequest) {
			.Request = SetReport,
			.Type = 0x21,
			.Index = interface,
			.Value = (u16)reportType << 8 | reportId,
			.Length = bufferLength,
		},
		HidMessageTimeout)) != OK) 
		return result;

	return OK;
}

static void BitSet(void* buffer, u32 offset, u32 length, u32 value) {
	u8* bitBuffer;
	u8 mask;

	bitBuffer = buffer;
	for (u32 i = offset / 8, j = 0; i < (offset + length + 7) / 8; i++) {
		if (offset / 8 == (offset + length - 1) / 8) {
			mask = (1 << ((offset % 8) + length)) - (1 << (offset % 8));		
			bitBuffer[i] = (bitBuffer[i] & ~mask) |
				((value << (offset % 8)) & mask);
		} else if (i == offset / 8) {
			mask = 0x100 - (1 << (offset % 8));		
			bitBuffer[i] = (bitBuffer[i] & ~mask) |
				((value << (offset % 8)) & mask);
			j += 8 - (offset % 8);
		} else if (i == (offset + length - 1) / 8) {
			mask = (1 << ((offset % 8) + length)) - 1;		
			bitBuffer[i] = (bitBuffer[i] & ~mask) |
				((value >> j) & mask);
		} else {			
			bitBuffer[i] = (value >> j) & 0xff;
			j += 8;
		}			 
	}
}

static u32 BitGetUnsigned(void* buffer, u32 offset, u32 length) {
	u8* bitBuffer;
	u8 mask;
	u32 result;
	
	bitBuffer = buffer;
	result = 0;
	for (u32 i = offset / 8, j = 0; i < (offset + length + 7) / 8; i++) {
		if (offset / 8 == (offset + length - 1) / 8) {
			mask = (1 << ((offset % 8) + length)) - (1 << (offset % 8));		
			result = (bitBuffer[i] & mask) >> (offset % 8);
		} else if (i == offset / 8) {
			mask = 0x100 - (1 << (offset % 8));		
			j += 8 - (offset % 8);
			result = ((bitBuffer[i] & mask) >> (offset % 8)) << (length - j);
		} else if (i == (offset + length - 1) / 8) {
			mask = (1 << ((offset % 8) + length)) - 1;		
			result |= bitBuffer[i] & mask;
		} else {			
			j += 8;
			result |= bitBuffer[i] << (length - j);
		}	
	}

	return result;
}

static s32 BitGetSigned(void* buffer, u32 offset, u32 length) {
	u32 result = BitGetUnsigned(buffer, offset, length);

	if (result & (1 << (length - 1)))
		result |= 0xffffffff - ((1 << length) - 1);

	return result;
}

static Result HidReadDevice(struct UsbDevice *device, u8 reportNumber) {
	struct HidDevice *data;
	struct HidParserResult *parse;
	struct HidParserReport *report;
	struct HidParserField *field;
	Result result;
	u32 size;
	
	data = (struct HidDevice*)device->DriverData;
	parse = data->ParserResult;
	report = parse->Report[reportNumber];
	size = ((report->ReportLength + 7) / 8);
	if ((report->ReportBuffer == NULL) && (report->ReportBuffer = (u8*)MemoryAllocate(size)) == NULL) {
		return ErrorMemory;
	}
	if ((result = HidGetReport(device, report->Type, report->Id, data->ParserResult->Interface, size, report->ReportBuffer)) != OK) {
		if (result != ErrorDisconnected)
			LOGF("HID: Could not read %s report %d.\n", UsbGetDescription(device), report);
		return result;
	}
	
	// Uncomment this for a quick hack to view 8 bytes worth of report.
	/*
	LOGF("HID: %s.Report%d: %02x%02x%02x%02x %02x%02x%02x%02x.\n", UsbGetDescription(device), reportNumber + 1,
		*(report->ReportBuffer + 0), *(report->ReportBuffer + 1), *(report->ReportBuffer + 2), *(report->ReportBuffer + 3),
		*(report->ReportBuffer + 4), *(report->ReportBuffer + 5), *(report->ReportBuffer + 6), *(report->ReportBuffer + 7));
	*/

	for (u32 i = 0; i < report->FieldCount; i++) {
		field = &report->Fields[i];
		if (field->Attributes.Variable) {
			if (field->LogicalMinimum < 0)
				field->Value.S32 = BitGetSigned(report->ReportBuffer, field->Offset, field->Size);
			else
				field->Value.U32 = BitGetUnsigned(report->ReportBuffer, field->Offset, field->Size);
		} else {
			for (u32 j = 0; j < field->Count; j++) {
				BitSet(
						field->Value.Pointer,
						j * field->Size, 
						field->Size, 
						field->LogicalMinimum < 0 ? BitGetSigned(
							report->ReportBuffer, 
							field->Offset + j * field->Size, 
							field->Size
						) : BitGetUnsigned(
							report->ReportBuffer, 
							field->Offset + j * field->Size, 
							field->Size
						)
					);				
			}
		}
	}

	return OK;
}

static Result HidWriteDevice(struct UsbDevice *device, u8 reportNumber) {
	struct HidDevice *data;
	struct HidParserResult *parse;
	struct HidParserReport *report;
	struct HidParserField *field;
	Result result;
	u32 size;
	
	data = (struct HidDevice*)device->DriverData;
	parse = data->ParserResult;
	report = parse->Report[reportNumber];
	size = ((report->ReportLength + 7) / 8);
	if ((report->ReportBuffer == NULL) && (report->ReportBuffer = (u8*)MemoryAllocate(size)) == NULL) {
		return ErrorMemory;
	}
	for (u32 i = 0; i < report->FieldCount; i++) {
		field = &report->Fields[i];
		if (field->Attributes.Variable) {
			BitSet(
					report->ReportBuffer, 
					field->Offset, 
					field->Size,
					field->Value.S32
				);
		} else {
			for (u32 j = 0; j < field->Count; j++) {
				BitSet(
						report->ReportBuffer,
						field->Offset + j * field->Size, 
						field->Size,
						BitGetSigned(
							field->Value.Pointer, 
							j * field->Size, 
							field->Size
						)
					);				
			}
		}
	}
	
	if ((result = HidSetReport(device, report->Type, report->Id, data->ParserResult->Interface, size, report->ReportBuffer)) != OK) {
		if (result != ErrorDisconnected)
			LOGF("HID: Coult not read %s report %d.\n", UsbGetDescription(device), report);
		return result;
	}

	return OK;
}

static Result HidSetProtocol(struct UsbDevice *device, u8 interface, u16 
    protocol) {
	Result result;
	
	if ((result = UsbControlMessage(
		device, 
		(struct UsbPipeAddress) { 
			.Type = Control, 
			.Speed = device->Speed, 
			.EndPoint = 0 , 
			.Device = device->Number, 
			.Direction = Out,
			.MaxSize = SizeFromNumber(device->Descriptor.MaxPacketSize0),
		},
		NULL,
		0,
		&(struct UsbDeviceRequest) {
			.Request = SetProtocol,
			.Type = 0x21,
			.Index = interface,
			.Value = protocol,
			.Length = 0,
		},
		HidMessageTimeout)) != OK) 
		return result;

	return OK;
}

static void HidEnumerateReport(void* descriptor, u16 length, 
    void(*action)(void* data, u16 tag, u32 value), void* data) {
	struct HidReportItem *item, *current;
	u16 parsedLength, currentIndex, currentLength;
	u16 tag; // tags for short items will be stored in the low 6 bits, tags for long items will be in the top 8 bits.
	s32 value;
	
	item = descriptor;	
	current = NULL;
	parsedLength = 0;
	
	while (parsedLength < length) {
		if (current == NULL) {
			current = item;
			currentIndex = 0;
			currentLength = 1 << (current->Size - 1);
			value = 0;
			tag = current->Tag;
			if (currentLength == 0)
				current = NULL;
		} else {
			if (current->Tag == TagLong && currentIndex < 2) {
				if (currentIndex == 0) currentLength += *(u8*)item;				
				else tag |= (u16)*(u8*)item << 8;
			} else {
				value |= (u32)*(u8*)item << (8 * currentIndex);
			}
			if (++currentIndex == currentLength)
				current = NULL;
		}

		if (current == NULL) {
			if ((tag & 0x3) == 0x1) {
				if (currentLength == 1 && (value & 0x80))
					value |= 0xffffff00;
				else if (currentLength == 2 && (value & 0x8000))
					value |= 0xffff0000;
			}

			action(data, tag, value);
		}

		item++;
		parsedLength++;
	}
}

#if DEBUG
static void HidEnumerateActionCountReport(void* data, u16 tag, u32 value) {
	struct {
		u8 reportCount;
		u8 indent;
		bool input, output, feature;
	} *reports = data;

	switch (tag) {
	case TagMainInput:
		if (!reports->input) { reports->reportCount++; reports->input = true; }
		LOG_DEBUGF("HID: %.*sInput(%03o)\n", reports->indent, "           ", value);
		break;
	case TagMainOutput:
		if (!reports->output) { reports->reportCount++; reports->output = true; }
		LOG_DEBUGF("HID: %.*sOutput(%03o)\n", reports->indent, "           ", value);
		break;
	case TagMainFeature:
		if (!reports->feature) { reports->reportCount++; reports->feature = true; }
		LOG_DEBUGF("HID: %.*sFeature(%03o)\n", reports->indent, "           ", value);
		break;
	case TagMainCollection:
		LOG_DEBUGF("HID: %.*sCollection(%d)\n", reports->indent, "           ", value);
		reports->indent++;
		break;
	case TagMainEndCollection:
		reports->indent--;
		LOG_DEBUGF("HID: %.*sEnd Collection\n", reports->indent, "           ");
		break;
	case TagGlobalUsagePage:
		LOG_DEBUGF("HID: %.*sUsage Page(%d)\n", reports->indent, "           ", value);
		break;
	case TagGlobalLogicalMinimum:
		LOG_DEBUGF("HID: %.*sLogical Minimum(%d)\n", reports->indent, "           ", value);
		break;
	case TagGlobalLogicalMaximum:
		LOG_DEBUGF("HID: %.*sLogical Maximum(%d)\n", reports->indent, "           ", value);
		break;
	case TagGlobalPhysicalMinimum:
		LOG_DEBUGF("HID: %.*sPhysical Minimum(%d)\n", reports->indent, "           ", value);
		break;
	case TagGlobalPhysicalMaximum:
		LOG_DEBUGF("HID: %.*sPhysical Maximum(%d)\n", reports->indent, "           ", value);
		break;
	case TagGlobalUnitExponent:
		LOG_DEBUGF("HID: %.*sUnit Exponent(%d)\n", reports->indent, "           ", value);
		break;
	case TagGlobalUnit:
		LOG_DEBUGF("HID: %.*sUnit(%d)\n", reports->indent, "           ", value);
		break;
	case TagGlobalReportSize:
		LOG_DEBUGF("HID: %.*sReport Size(%d)\n", reports->indent, "           ", value);
		break;
	case TagGlobalReportId:
		reports->input = reports->output = reports->feature = false;
		LOG_DEBUGF("HID: %.*sReport ID(%d)\n", reports->indent, "           ", value);
		break;
	case TagGlobalReportCount:
		LOG_DEBUGF("HID: %.*sReport Count(%d)\n", reports->indent, "           ", value);
		break;
	case TagGlobalPush:
		LOG_DEBUGF("HID: %.*sPush\n", reports->indent, "           ");
		break;
	case TagGlobalPop:
		LOG_DEBUGF("HID: %.*sPop\n", reports->indent, "           ");
		break;
	case TagLocalUsage:
		LOG_DEBUGF("HID: %.*sUsage(%u)\n", reports->indent, "           ", value);
		break;
	case TagLocalUsageMinimum:
		LOG_DEBUGF("HID: %.*sUsage Minimum(%u)\n", reports->indent, "           ", value);
		break;
	case TagLocalUsageMaximum:
		LOG_DEBUGF("HID: %.*sUsage Maximum(%u)\n", reports->indent, "           ", value);
		break;
	case TagLocalDesignatorIndex:
		LOG_DEBUGF("HID: %.*sDesignator Index(%u)\n", reports->indent, "           ", value);
		break;
	case TagLocalDesignatorMinimum:
		LOG_DEBUGF("HID: %.*sDesignator Minimum(%u)\n", reports->indent, "           ", value);
		break;
	case TagLocalDesignatorMaximum:
		LOG_DEBUGF("HID: %.*sDesignator Maximum(%u)\n", reports->indent, "           ", value);
		break;
	case TagLocalStringIndex:
		LOG_DEBUGF("HID: %.*sString Index(%u)\n", reports->indent, "           ", value);
		break;
	case TagLocalStringMinimum:
		LOG_DEBUGF("HID: %.*sString Minimum(%u)\n", reports->indent, "           ", value);
		break;
	case TagLocalStringMaximum:
		LOG_DEBUGF("HID: %.*sString Maximum(%u)\n", reports->indent, "           ", value);
		break;
	case TagLocalDelimiter:
		LOG_DEBUGF("HID: %.*sDelimiter\n", reports->indent, "           ");
		break;
	default:
		LOG_DEBUGF("HID: Unexpected tag in report %d = %x.\n", tag, value);
		break;
	}
}
#else
static void HidEnumerateActionCountReport(void* data, u16 tag, u32 value) {
	struct {
		u8 reportCount;
		u8 indent;
		bool input, output, feature;
	} *reports = data;

	switch (tag) {
	case TagMainInput:
		if (!reports->input) { reports->reportCount++; reports->input = true; }
		break;
	case TagMainOutput:
		if (!reports->output) { reports->reportCount++; reports->output = true; }
		break;
	case TagMainFeature:
		if (!reports->feature) { reports->reportCount++; reports->feature = true; }
		break;
	case TagGlobalReportId:
		reports->input = reports->output = reports->feature = false;
		break;
	default:
		break;
	}
}
#endif

static void HidEnumerateActionCountField(void* data, u16 tag, u32 value) {
	struct reportFields_t {
		u32 count;
		u8 current;
		u8 report;
		struct reportFields_inner_t {
			u8 reportId;
			u8 fieldCount;
			enum HidReportType type;
		} reports[];
	} *reportFields = data;
	struct reportFields_inner_t *inner;
	enum HidReportType type;
	
	type = 0;
	switch (tag) {
	case TagMainFeature: type++;
	case TagMainOutput: type++;
	case TagMainInput: type++;
		inner = NULL;
		for (u32 i = 0; i < reportFields->current; i++) {
			if (reportFields->reports[i].reportId == reportFields->report &&
				reportFields->reports[i].type == type) {
				inner = &reportFields->reports[i];
				break; 
			}
		}
		if (inner == NULL) {
			inner = &reportFields->reports[reportFields->current++];
			inner->reportId = reportFields->report;
			inner->fieldCount = 0;
			inner->type = type;
		}
		if (((struct HidMainItem*)&value)->Variable) inner->fieldCount += reportFields->count;
		else inner->fieldCount++;
		break;
	case TagGlobalReportCount:
		reportFields->count = value;
		break;
	case TagGlobalReportId:
		reportFields->report = value;
		break;
	default: break;
	}
}

static void HidEnumerateActionAddField(void* data, u16 tag, u32 value) {
	struct fields_t {
		struct HidParserResult *result;
		u32 count;
		u32 size;
		struct HidFullUsage *usage;
		struct HidFullUsage physical;
		s32 logicalMinimum;
		s32 logicalMaximum;	
		s32 physicalMinimum;
		s32 physicalMaximum;
		struct HidUnit unit;
		s32 unitExponent;		
		enum HidUsagePage page;
		u8 report;
	} *fields = data;
	enum HidReportType type;
	struct HidParserReport *report;
	u32 i;

	type = 0;
	switch (tag) {
	case TagMainFeature: type++;
	case TagMainOutput: type++;
	case TagMainInput: type++;
		report = NULL;
		for (i = 0; i < fields->result->ReportCount; i++)
			if (fields->result->Report[i]->Id == fields->report &&
				fields->result->Report[i]->Type == type) {
				report = fields->result->Report[i];
				break; 
			}
		while (fields->count > 0) {
			if (*(u32*)fields->usage == 0xffffffff) fields->usage++;
			*(u32*)&(report->Fields[report->FieldCount].Attributes) = value;
			report->Fields[report->FieldCount].Count = report->Fields[report->FieldCount].Attributes.Variable ? 1 : fields->count;
			report->Fields[report->FieldCount].LogicalMaximum = fields->logicalMaximum;
			report->Fields[report->FieldCount].LogicalMinimum = fields->logicalMinimum;
			report->Fields[report->FieldCount].Offset = report->ReportLength;
			report->Fields[report->FieldCount].PhysicalMaximum = fields->physicalMaximum;
			report->Fields[report->FieldCount].PhysicalMinimum = fields->physicalMinimum;
			*(u32*)&report->Fields[report->FieldCount].PhysicalUsage = *(u32*)&fields->physical;
			report->Fields[report->FieldCount].Size = fields->size;
			*(u32*)&report->Fields[report->FieldCount].Unit = *(u32*)&fields->unit;
			report->Fields[report->FieldCount].UnitExponent = fields->unitExponent;
			if ((u16)fields->usage->Page == 0xffff) {
				*(u32*)&report->Fields[report->FieldCount].Usage = *(u32*)&fields->usage[-1];
				if (fields->usage->Desktop == fields->usage[-1].Desktop ||
					!report->Fields[report->FieldCount].Attributes.Variable)
					fields->usage -= 2;
				else
					fields->usage[-1].Desktop++;
			} else {
				*(u32*)&report->Fields[report->FieldCount].Usage = *(u32*)(fields->usage--);
			}
			if (report->Fields[report->FieldCount].Attributes.Variable) {
				fields->count--;
				report->ReportLength += report->Fields[report->FieldCount].Size;
				report->Fields[report->FieldCount].Value.U32 = 0;
			}
			else {
				fields->count = 0;
				report->ReportLength += report->Fields[report->FieldCount].Size * report->Fields[report->FieldCount].Count;
				report->Fields[report->FieldCount].Value.Pointer = MemoryAllocate(report->Fields[report->FieldCount].Size * report->Fields[report->FieldCount].Count / 8);
			}
			report->FieldCount++;
		}
		*(u32*)&fields->usage[1] = 0;
		break;
	case TagMainCollection:
		if (*(u32*)fields->usage == 0xffffffff) fields->usage++;
		switch ((enum HidMainCollection)value) {
		case Application:			
			*(u32*)&fields->result->Application = *(u32*)fields->usage;
			break;
		case Physical:
			*(u32*)&fields->physical = *(u32*)fields->usage;
			break;
		default:
			break;
		}
		fields->usage--;
		break;
	case TagMainEndCollection:
		switch ((enum HidMainCollection)value) {
		case Physical:
			*(u32*)&fields->physical = 0;
			break;
		default:
			break;
		}
		break;
	case TagGlobalUsagePage:		
		fields->page = (enum HidUsagePage)value;
		break;
	case TagGlobalLogicalMinimum:
		fields->logicalMinimum = value;
		break;
	case TagGlobalLogicalMaximum:
		fields->logicalMaximum = value;
		break;
	case TagGlobalPhysicalMinimum:
		fields->physicalMinimum = value;
		break;
	case TagGlobalPhysicalMaximum:
		fields->physicalMaximum = value;
		break;
	case TagGlobalUnitExponent:
		fields->unitExponent = value;
		break;
	case TagGlobalUnit:
		*(u32*)&fields->unit = value;
		break;
	case TagGlobalReportSize:
		fields->size = value;
		break;
	case TagGlobalReportId:
		fields->report = (u8)value;
		break;
	case TagGlobalReportCount:
		fields->count = value;
		break;
	case TagLocalUsage:
		fields->usage++;
		if (value & 0xffff0000)
			*(u32*)&fields->usage = value;
		else {
			fields->usage->Desktop = (enum HidUsagePageDesktop)value;
			fields->usage->Page = fields->page;
		}
		break;
	case TagLocalUsageMinimum:
		fields->usage++;
		if (value & 0xffff0000)
			*(u32*)&fields->usage = value;
		else {
			fields->usage->Desktop = (enum HidUsagePageDesktop)value;
			fields->usage->Page = fields->page;
		}
		break;
	case TagLocalUsageMaximum:
		fields->usage++;
		fields->usage->Desktop = (enum HidUsagePageDesktop)value;
		fields->usage->Page = (enum HidUsagePage)0xffff;
		break;
	default: break;
	}
}

static Result HidParseReportDescriptor(struct UsbDevice *device, void* 
    descriptor, u16 length) {
	Result result;
	struct HidDevice *data;
	struct HidParserResult *parse = NULL;
	struct {
		u8 reportCount;
		u8 indent;
		bool input, output, feature;
	} reports = { .reportCount = 0, .indent = 0, .input = false, .output = false, .feature = false };
	struct reportFields_t {
		u32 count;
		u8 current;
		u8 report;
		struct reportFields_inner_t {
			u8 reportId;
			u8 fieldCount;
			enum HidReportType type;
		} reports[];
	} *reportFields = NULL;
	struct fields_t {
		struct HidParserResult *result;
		u32 count;
		u32 size;
		struct HidFullUsage *usage;
		struct HidFullUsage physical;
		s32 logicalMinimum;
		s32 logicalMaximum;	
		s32 physicalMinimum;
		s32 physicalMaximum;
		struct HidUnit unit;
		s32 unitExponent;		
		enum HidUsagePage page;
		u8 report;
	} *fields = NULL;
	void* usageStack = NULL;

	data = (struct HidDevice*)device->DriverData;

	HidEnumerateReport(descriptor, length, HidEnumerateActionCountReport, &reports);
	LOG_DEBUGF("HID: Found %d reports.", reports.reportCount);

	if ((parse = MemoryAllocate(sizeof(struct HidParserResult) + 4 * reports.reportCount)) == NULL) {
		result = ErrorMemory;
		goto deallocate;
	}
	for (u32 i = 0; i < reports.reportCount; i++) {
		parse->Report[i] = NULL;
	}
	if ((reportFields = MemoryAllocate(sizeof(struct reportFields_t) + sizeof(struct reportFields_inner_t) * reports.reportCount)) == NULL) {
		result = ErrorMemory;
		goto deallocate;
	}
	parse->ReportCount = reports.reportCount;
	reportFields->count = 0;
	reportFields->current = 0;
	reportFields->report = 0;

	HidEnumerateReport(descriptor, length, HidEnumerateActionCountField, reportFields);
	for (u32 i = 0; i < reports.reportCount; i++) {
		if ((parse->Report[i] = MemoryAllocate(sizeof(struct HidParserReport) + sizeof(struct HidParserField) * reportFields->reports[i].fieldCount)) == NULL) {
			result = ErrorMemory;
			goto deallocate;
		}
		parse->Report[i]->Index = i;
		parse->Report[i]->FieldCount = 0;
		parse->Report[i]->Id = reportFields->reports[i].reportId;
		parse->Report[i]->Type = reportFields->reports[i].type;
		parse->Report[i]->ReportLength = 0;
		parse->Report[i]->ReportBuffer = NULL;
	}	
	MemoryDeallocate(reportFields);
	reportFields = NULL;

	if ((fields = MemoryAllocate(sizeof(struct fields_t))) == NULL) {
		result = ErrorMemory;
		goto deallocate;
	}
	if ((fields->usage = usageStack = MemoryAllocate(16 * sizeof(struct HidFullUsage*))) == NULL) {
		result = ErrorMemory;
		goto deallocate;
	}	
	fields->count = 0;
	fields->logicalMaximum = 0;
	fields->logicalMinimum = 0;
	fields->physicalMaximum = 0;
	fields->physicalMinimum = 0;
	fields->report = 0;
	fields->size = 0;
	*(u32*)fields->usage = 0xffffffff;
	fields->result = parse;
	HidEnumerateReport(descriptor, length, HidEnumerateActionAddField, fields);
	
	data->ParserResult = parse;
	parse = NULL;
	result = OK;
deallocate:
	if (usageStack != NULL) MemoryDeallocate(usageStack);
	if (fields != NULL) MemoryDeallocate(fields);
	if (reportFields != NULL) MemoryDeallocate(reportFields);
	if (parse != NULL) {
		for (u32 i = 0; i < reports.reportCount; i++)
			if (parse->Report[i] != NULL) MemoryDeallocate(parse->Report[i]);
		MemoryDeallocate(parse);
	}
	return result;
}

static void HidDetached(struct UsbDevice *device) {
	struct HidDevice *data;
	
	if (device->DriverData != NULL) {
		data = (struct HidDevice*)device->DriverData;

		if (data->HidDetached != NULL)
			data->HidDetached(device);
	}
}

static void HidDeallocate(struct UsbDevice *device) {
	struct HidDevice *data;
	struct HidParserReport *report;
	
	if (device->DriverData != NULL) {
		data = (struct HidDevice*)device->DriverData;

		if (data->HidDeallocate != NULL)
			data->HidDeallocate(device);

		if (data->ParserResult != NULL) {
			for (u32 i = 0; i < data->ParserResult->ReportCount; i++) {
				if (data->ParserResult->Report[i] != NULL) {
					report = data->ParserResult->Report[i];
					if (report->ReportBuffer != NULL)
						MemoryDeallocate(report->ReportBuffer);
					for (u32 j = 0; j < report->FieldCount; j++) 
						if (!report->Fields[j].Attributes.Variable)
							MemoryDeallocate(report->Fields[j].Value.Pointer);
					MemoryDeallocate(data->ParserResult->Report[i]);
				}
			}
			MemoryDeallocate(data->ParserResult);
		}

		MemoryDeallocate(data);
	}
	device->DeviceDeallocate = NULL;
	device->DeviceDetached = NULL;
}

static Result HidAttach(struct UsbDevice *device, u32 interfaceNumber) {
	struct HidDevice *data;
	struct HidDescriptor *descriptor;
	struct UsbDescriptorHeader *header;
	void* reportDescriptor = NULL;
	Result result;
	u32 currentInterface;

	if (device->Interfaces[interfaceNumber].Class != InterfaceClassHid) {
		return ErrorArgument;
	}
	if (device->Interfaces[interfaceNumber].EndpointCount < 1) {
		LOGF("HID: Invalid HID device with fewer than one endpoints (%d).\n", device->Interfaces[interfaceNumber].EndpointCount);
		return ErrorIncompatible;
	}
	if (device->Endpoints[interfaceNumber][0].EndpointAddress.Direction != In ||
		device->Endpoints[interfaceNumber][0].Attributes.Type != Interrupt) {
		LOG("HID: Invalid HID device with unusual endpoints (0).\n");
		return ErrorIncompatible;
	}
	if (device->Interfaces[interfaceNumber].EndpointCount >= 2) {
		if (device->Endpoints[interfaceNumber][1].EndpointAddress.Direction != Out ||
			device->Endpoints[interfaceNumber][1].Attributes.Type != Interrupt) {
			LOG("HID: Invalid HID device with unusual endpoints (1).\n");
			return ErrorIncompatible;
		}	
	}
	if (device->Status != Configured) {
		LOG("HID: Cannot start driver on unconfigured device!\n");
		return ErrorDevice;
	}
	if (device->Interfaces[interfaceNumber].SubClass == 1) {
		if (device->Interfaces[interfaceNumber].Protocol == 1)
			LOG_DEBUG("HID: Boot keyboard detected.\n");
		else if (device->Interfaces[interfaceNumber].Protocol == 2)
			LOG_DEBUG("HID: Boot mouse detected.\n");
		else 
			LOG_DEBUG("HID: Unknown boot device detected.\n");
		
		LOG_DEBUG("HID: Reverting from boot to normal HID mode.\n");
		if ((result = HidSetProtocol(device, interfaceNumber, 1)) != OK) {
			LOG("HID: Could not revert to report mode from HID mode.\n");
			return result;
		}
	}

	header = (struct UsbDescriptorHeader*)device->FullConfiguration;
	descriptor = NULL;
	currentInterface = interfaceNumber + 1; // Certainly different!
	do {		
		if (header->DescriptorLength == 0) break; // List end
		switch (header->DescriptorType) {
		case Interface:
			currentInterface = ((struct UsbInterfaceDescriptor*)header)->Number;
			break;
		case Hid:
			if (currentInterface == interfaceNumber)
				descriptor = (void*)header;
			break;
		default:
			break;
		}
		
		LOG_DEBUGF("HID: Descriptor %d length %d, interface %d.\n", header->DescriptorType, header->DescriptorLength, currentInterface);

		if (descriptor != NULL) break;
		header = (void*)((u8*)header + header->DescriptorLength);
	} while (true);

	if (descriptor == NULL) {
		LOGF("HID: No HID descriptor in %s.Interface%d. Cannot be a HID device.\n", UsbGetDescription(device), interfaceNumber + 1);
		return ErrorIncompatible;
	}

	if (descriptor->HidVersion > 0x111) {
		LOGF("HID: Device uses unsupported HID version %x.%x.\n", descriptor->HidVersion >> 8, descriptor->HidVersion & 0xff);
		return ErrorIncompatible;
	}
	LOG_DEBUGF("HID: Device version HID %x.%x.\n", descriptor->HidVersion >> 8, descriptor->HidVersion & 0xff);
	
	device->DeviceDeallocate = HidDeallocate;
	device->DeviceDetached = HidDetached;
	if ((device->DriverData = MemoryAllocate(sizeof (struct HidDevice))) == NULL) {
		result = ErrorMemory;
		goto deallocate;
	}
	device->DriverData->DataSize = sizeof(struct HidDevice);
	device->DriverData->DeviceDriver = DeviceDriverHid;
	data = (struct HidDevice*)device->DriverData;
	data->Descriptor = descriptor;
	data->DriverData = NULL;
	
	if ((reportDescriptor = MemoryAllocate(descriptor->OptionalDescriptors[0].Length)) == NULL) {
		result = ErrorMemory;
		goto deallocate;
	}
	if ((result = UsbGetDescriptor(device, HidReport, 0, interfaceNumber, reportDescriptor, descriptor->OptionalDescriptors[0].Length, descriptor->OptionalDescriptors[0].Length, 1)) != OK) {
		MemoryDeallocate(reportDescriptor);
		LOGF("HID: Could not read report descriptor for %s.Interface%d.\n", UsbGetDescription(device), interfaceNumber + 1);
		goto deallocate;
	}
	if ((result = HidParseReportDescriptor(device, reportDescriptor, descriptor->OptionalDescriptors[0].Length)) != OK) {		
		MemoryDeallocate(reportDescriptor);
		LOGF("HID: Invalid report descriptor for %s.Interface%d.\n", UsbGetDescription(device), interfaceNumber + 1);
		goto deallocate;
	}

	MemoryDeallocate(reportDescriptor);
	reportDescriptor = NULL;

	data->ParserResult->Interface = interfaceNumber;
	if (data->ParserResult->Application.Page == GenericDesktopControl &&
		(u16)data->ParserResult->Application.Desktop < HidUsageAttachCount &&
		HidUsageAttach[(u16)data->ParserResult->Application.Desktop] != NULL) {
		HidUsageAttach[(u16)data->ParserResult->Application.Desktop](device, interfaceNumber);
	}

	return OK;
deallocate:
	if (reportDescriptor != NULL) MemoryDeallocate(reportDescriptor);
	HidDeallocate(device);
	return result;
}

static s32 HidGetFieldValue(struct HidParserField *field, u32 index) {
	return BitGetSigned(field->Value.Pointer, index * field->Size, field->Size);
}
/******************************************************************************
*	device/hid/keyboard.c
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	device/hid/keyboard.c contains code relating to USB hid keyboards. The 
*	driver maintains a list of the keyboards on the system, and allows the 
*	operating system to retrieve the status of each one separately. It is coded
*	a little awkwardly on purpose to make OS development more fun!
******************************************************************************/

#define KeyboardMaxKeyboards 4

static u32 keyboardCount __attribute__((aligned(4))) = 0;
static u32 keyboardAddresses[KeyboardMaxKeyboards] = { 0, 0, 0, 0 };
static struct UsbDevice* keyboards[KeyboardMaxKeyboards] = {0,0,0,0};

static void KbdLoad() {
	LOG_DEBUG("CSUD: Keyboard driver version 0.1\n"); 
	keyboardCount = 0;
	for (u32 i = 0; i < KeyboardMaxKeyboards; i++)
	{
		keyboardAddresses[i] = 0;
		keyboards[i] = NULL;
	}
	HidUsageAttach[DesktopKeyboard] = KeyboardAttach;
}

static u32 KeyboardIndex(u32 address) {
	if (address == 0) return 0xffffffff;

	for (u32 i = 0; i < keyboardCount; i++)
		if (keyboardAddresses[i] == address)
			return i;

	return 0xffffffff;
}

unsigned int KeyboardGetAddress(u32 index) {
	if (index > keyboardCount) return 0;

	for (u32 i = 0; index >= 0 && i < KeyboardMaxKeyboards; i++) {
		if (keyboardAddresses[i] != 0)
			if (index-- == 0)
				return keyboardAddresses[i];
	}

	return 0;
}

static void KeyboardDetached(struct UsbDevice *device) {
	struct KeyboardDevice *data;
	
	data = (struct KeyboardDevice*)((struct HidDevice*)device->DriverData)->DriverData;
	if (data != NULL) {
		if (keyboardAddresses[data->Index] == device->Number) {
			keyboardAddresses[data->Index] = 0;
			keyboardCount--;
			keyboards[data->Index] = NULL;
		}
	}
}

static void KeyboardDeallocate(struct UsbDevice *device) {
	struct KeyboardDevice *data;
	
	data = (struct KeyboardDevice*)((struct HidDevice*)device->DriverData)->DriverData;
	if (data != NULL) {
		MemoryDeallocate(data);
		((struct HidDevice*)device->DriverData)->DriverData = NULL;
	}
	((struct HidDevice*)device->DriverData)->HidDeallocate = NULL;
	((struct HidDevice*)device->DriverData)->HidDetached = NULL;
}

static Result KeyboardAttach(struct UsbDevice *device, u32 interface) {
	u32 keyboardNumber;
	struct HidDevice *hidData;
	struct KeyboardDevice *data;
	struct HidParserResult *parse;

	if ((KeyboardMaxKeyboards & 3) != 0) {
		LOG("KBD: Warning! KeyboardMaxKeyboards not a multiple of 4. The driver wasn't built for this!\n");
	}
	if (keyboardCount == KeyboardMaxKeyboards) {
		LOGF("KBD: %s not connected. Too many keyboards connected (%d/%d). Change KeyboardMaxKeyboards in device.keyboard.c to allow more.\n", UsbGetDescription(device), keyboardCount, KeyboardMaxKeyboards);
		return ErrorIncompatible;
	}
	
	hidData = (struct HidDevice*)device->DriverData;
	if (hidData->Header.DeviceDriver != DeviceDriverHid) {
		LOGF("KBD: %s isn't a HID device. The keyboard driver is built upon the HID driver.\n", UsbGetDescription(device));
		return ErrorIncompatible;
	}

	parse = hidData->ParserResult;
	if ((parse->Application.Page != GenericDesktopControl && parse->Application.Page != Undefined) ||
		parse->Application.Desktop != DesktopKeyboard) {
		LOGF("KBD: %s doesn't seem to be a keyboard (%x != %x || %x != %x)...\n", UsbGetDescription(device), parse->Application.Page, GenericDesktopControl, parse->Application.Desktop, DesktopKeyboard);
		return ErrorIncompatible;
	}
	if (parse->ReportCount < 1) {
		LOGF("KBD: %s doesn't have enough outputs to be a keyboard.\n", UsbGetDescription(device));
		return ErrorIncompatible;
	}
	hidData->HidDetached = KeyboardDetached;
	hidData->HidDeallocate = KeyboardDeallocate;
	if ((hidData->DriverData = MemoryAllocate(sizeof(struct KeyboardDevice))) == NULL) {
		LOGF("KBD: Not enough memory to allocate keyboard %s.\n", UsbGetDescription(device));
		return ErrorMemory;
	}
	data = (struct KeyboardDevice*)hidData->DriverData;
	data->Header.DeviceDriver = DeviceDriverKeyboard;
	data->Header.DataSize = sizeof(struct KeyboardDevice);
	data->Index = keyboardNumber = 0xffffffff;
	for (u32 i = 0; i < KeyboardMaxKeyboards; i++) {
		if (keyboardAddresses[i] == 0) {
			data->Index = keyboardNumber = i;
			keyboardAddresses[i] = device->Number;
			keyboardCount++;
			break;
		}
	}

	if (keyboardNumber == 0xffffffff) {
		LOG("KBD: PANIC! Driver in inconsistent state! KeyboardCount is inaccurate.\n");
		KeyboardDeallocate(device);
		return ErrorGeneral;
	}

	keyboards[keyboardNumber] = device;
	for (u32 i = 0; i < KeyboardMaxKeys; i++)
		data->Keys[i] = 0;
	*(u8*)&data->Modifiers = 0;
	*(u8*)&data->LedSupport = 0;

	for (u32 i = 0; i < 9; i++)
		data->KeyFields[i] = NULL;
	for (u32 i = 0; i < 8; i++)
		data->LedFields[i] = NULL;
	data->LedReport = NULL;
	data->KeyReport = NULL;

	for (u32 i = 0; i < parse->ReportCount; i++) {
    LOG_DEBUGF("KBD: type %x report %d. %d fields.\n", parse->Report[i]->Type, i, parse->Report[i]->FieldCount);
		if (parse->Report[i]->Type == Input && 
			data->KeyReport == NULL) {
			LOG_DEBUGF("KBD: Output report %d. %d fields.\n", i, parse->Report[i]->FieldCount);
			data->KeyReport = parse->Report[i];
			for (u32 j = 0; j < parse->Report[i]->FieldCount; j++) {
				if (parse->Report[i]->Fields[j].Usage.Page == KeyboardControl || parse->Report[i]->Fields[j].Usage.Page == Undefined) {
					if (parse->Report[i]->Fields[j].Attributes.Variable) {
						if (parse->Report[i]->Fields[j].Usage.Keyboard >= KeyboardLeftControl
							&& parse->Report[i]->Fields[j].Usage.Keyboard <= KeyboardRightGui)
							LOG_DEBUGF("KBD: Modifier %d detected! Offset=%x, size=%x\n", parse->Report[i]->Fields[j].Usage.Keyboard, parse->Report[i]->Fields[j].Offset, parse->Report[i]->Fields[j].Size);
							data->KeyFields[(u16)parse->Report[i]->Fields[j].Usage.Keyboard - (u16)KeyboardLeftControl] = 
								&parse->Report[i]->Fields[j];
					} else {
						LOG_DEBUG("KBD: Key input detected!\n");
						data->KeyFields[8] = &parse->Report[i]->Fields[j];
					}
				}
			}
		} else if (parse->Report[i]->Type == Output && 
			data->LedReport == NULL) {
			data->LedReport = parse->Report[i];
			LOG_DEBUGF("KBD: Input report %d. %d fields.\n", i, parse->Report[i]->FieldCount);
			for (u32 j = 0; j < parse->Report[i]->FieldCount; j++) {
				if (parse->Report[i]->Fields[j].Usage.Page == Led) {
					switch (parse->Report[i]->Fields[j].Usage.Led) {
					case LedNumberLock:
						LOG_DEBUG("KBD: Number lock LED detected!\n");
						data->LedFields[0] = &parse->Report[i]->Fields[j];
						data->LedSupport.NumberLock = true;
						break;
					case LedCapsLock:
						LOG_DEBUG("KBD: Caps lock LED detected!\n");
						data->LedFields[1] = &parse->Report[i]->Fields[j];
						data->LedSupport.CapsLock = true;
						break;
					case LedScrollLock:
						LOG_DEBUG("KBD: Scroll lock LED detected!\n");
						data->LedFields[2] = &parse->Report[i]->Fields[j];
						data->LedSupport.ScrollLock = true;
						break;
					case LedCompose:
						LOG_DEBUG("KBD: Compose LED detected!\n");
						data->LedFields[3] = &parse->Report[i]->Fields[j];
						data->LedSupport.Compose = true;
						break;
					case LedKana:
						LOG_DEBUG("KBD: Kana LED detected!\n");
						data->LedFields[4] = &parse->Report[i]->Fields[j];
						data->LedSupport.Kana = true;
						break;
					case LedPower:
						LOG_DEBUG("KBD: Power LED detected!\n");
						data->LedFields[5] = &parse->Report[i]->Fields[j];
						data->LedSupport.Power = true;
						break;
					case LedShift:
						LOG_DEBUG("KBD: Shift LED detected!\n");
						data->LedFields[6] = &parse->Report[i]->Fields[j];
						data->LedSupport.Shift = true;
						break;
					case LedMute:
						LOG_DEBUG("KBD: Mute LED detected!\n");
						data->LedFields[7] = &parse->Report[i]->Fields[j];
						data->LedSupport.Mute = true;
						break;
					default: break; 
					}
				}
			}
		}
	}

	LOG_DEBUGF("KBD: New keyboard assigned %d!\n", device->Number);

	return OK;
}

int KeyboardCount(void) {
	return keyboardCount;
}

static Result KeyboardSetLeds(u32 keyboardAddress, struct KeyboardLeds leds) {
	u32 keyboardNumber;
	struct KeyboardDevice *data;

	keyboardNumber = KeyboardIndex(keyboardAddress);
	if (keyboardNumber == 0xffffffff) return ErrorDisconnected;
	data = (struct KeyboardDevice*)((struct HidDevice*)keyboards[keyboardNumber]->DriverData)->DriverData;

	if (data->LedSupport.NumberLock)
		data->LedFields[0]->Value.Bool = leds.NumberLock;
	if (data->LedSupport.CapsLock)
		data->LedFields[1]->Value.Bool = leds.CapsLock;
	if (data->LedSupport.ScrollLock)
		data->LedFields[2]->Value.Bool = leds.ScrollLock;
	if (data->LedSupport.Compose)
		data->LedFields[3]->Value.Bool = leds.Compose;
	if (data->LedSupport.Kana)
		data->LedFields[4]->Value.Bool = leds.Kana;
	if (data->LedSupport.Power)
		data->LedFields[5]->Value.Bool = leds.Power;
	if (data->LedSupport.Shift)
		data->LedFields[6]->Value.Bool = leds.Shift;
	if (data->LedSupport.Mute)
		data->LedFields[7]->Value.Bool = leds.Mute;

	return HidWriteDevice(keyboards[keyboardNumber], data->LedReport->Index);
}

struct KeyboardLeds KeyboardGetLedSupport(u32 keyboardAddress) {
	u32 keyboardNumber;
	struct KeyboardDevice *data;
	
	keyboardNumber = KeyboardIndex(keyboardAddress);
	if (keyboardNumber == 0xffffffff) return (struct KeyboardLeds) { };
	data = (struct KeyboardDevice*)((struct HidDevice*)keyboards[keyboardNumber]->DriverData)->DriverData;
	return data->LedSupport;
}

Result KeyboardPoll(unsigned int keyboardAddress) {
	u32 keyboardNumber;
	Result result;
	struct KeyboardDevice *data;
	
	keyboardNumber = KeyboardIndex(keyboardAddress);	
	if (keyboardNumber == 0xffffffff) return ErrorDisconnected;
	data = (struct KeyboardDevice*)((struct HidDevice*)keyboards[keyboardNumber]->DriverData)->DriverData;
	if ((result = HidReadDevice(keyboards[keyboardNumber], data->KeyReport->Index)) != OK) {
		if (result != ErrorDisconnected)
			LOGF("KBD: Could not get key report from %s.\n", UsbGetDescription(keyboards[keyboardNumber]));
		return result;
	}

	if (data->KeyFields[0] != NULL)
		data->Modifiers.LeftControl = data->KeyFields[0]->Value.Bool;
	if (data->KeyFields[1] != NULL)
		data->Modifiers.LeftShift = data->KeyFields[1]->Value.Bool;
	if (data->KeyFields[2] != NULL)
		data->Modifiers.LeftAlt = data->KeyFields[2]->Value.Bool;
	if (data->KeyFields[3] != NULL)
		data->Modifiers.LeftGui = data->KeyFields[3]->Value.Bool;
	if (data->KeyFields[4] != NULL)
		data->Modifiers.RightControl = data->KeyFields[4]->Value.Bool;
	if (data->KeyFields[5] != NULL)
		data->Modifiers.RightShift = data->KeyFields[5]->Value.Bool;
	if (data->KeyFields[6] != NULL)
		data->Modifiers.RightAlt = data->KeyFields[6]->Value.Bool;
	if (data->KeyFields[7] != NULL)
		data->Modifiers.RightGui = data->KeyFields[7]->Value.Bool;
	if (data->KeyFields[8] != NULL) {
		if (HidGetFieldValue(data->KeyFields[8], 0) != KeyboardErrorRollOver) {
			data->KeyCount = 0;
			for (u32 i = 0; i < KeyboardMaxKeys && i < data->KeyFields[8]->Count; i++) {
				if ((data->Keys[i] = HidGetFieldValue(data->KeyFields[8], i)) + (u16)data->KeyFields[8]->Usage.Keyboard != 0)
					data->KeyCount++;
			}
		}
	}

	return OK;
}

static struct KeyboardModifiers KeyboardGetModifiers(u32 keyboardAddress) {
	u32 keyboardNumber;
	struct KeyboardDevice *data;
	
	keyboardNumber = KeyboardIndex(keyboardAddress);
	if (keyboardNumber == 0xffffffff) return (struct KeyboardModifiers) { };
	data = (struct KeyboardDevice*)((struct HidDevice*)keyboards[keyboardNumber]->DriverData)->DriverData;
	return data->Modifiers;
}

static u32 KeyboardGetKeyDownCount(u32 keyboardAddress) {
	u32 keyboardNumber;
	struct KeyboardDevice *data;
	
	keyboardNumber = KeyboardIndex(keyboardAddress);
	if (keyboardNumber == 0xffffffff) return 0;
	data = (struct KeyboardDevice*)((struct HidDevice*)keyboards[keyboardNumber]->DriverData)->DriverData;
	return data->KeyCount;
}

static bool KeyboadGetKeyIsDown(u32 keyboardAddress, u16 key) {
	u32 keyboardNumber;
	struct KeyboardDevice *data;
	
	keyboardNumber = KeyboardIndex(keyboardAddress);
	if (keyboardNumber == 0xffffffff) return false;
	data = (struct KeyboardDevice*)((struct HidDevice*)keyboards[keyboardNumber]->DriverData)->DriverData;
	for (u32 i = 0; i < data->KeyCount; i++)
		if (data->Keys[i] == key) return true;
	return false;
}

u16 KeyboardGetKeyDown(u32 keyboardAddress, u32 index) {
	u32 keyboardNumber;
	struct KeyboardDevice *data;
	u32 keyCount = KeyboardGetKeyDownCount(keyboardAddress);
	
	keyboardNumber = KeyboardIndex(keyboardAddress);
	if (keyboardNumber == 0xffffffff) return 0;
	data = (struct KeyboardDevice*)((struct HidDevice*)keyboards[keyboardNumber]->DriverData)->DriverData;
	if (index >= keyCount) return 0;
	return data->Keys[index];
}
/******************************************************************************
*	device/mouse.c
*	 by Steve White 
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	device/mouse.c contains code relating to USB hid mice. The 
*	driver maintains a list of the mice on the system, and allows the 
*	operating system to retrieve the status of each one separately. It is coded
*	a little awkwardly on purpose to make OS development more fun!
******************************************************************************/

#define MouseMaxMice 4

static u32 mouseCount __attribute__((aligned(4))) = 0;
static u32 mouseAddresses[MouseMaxMice] = { 0, 0, 0, 0 };
static struct UsbDevice* mice[MouseMaxMice] = {0,0,0,0};

void MouseLoad() 
{
	LOG_DEBUG("CSUD: Mouse driver version 0.1\n"); 
	mouseCount = 0;
	for (u32 i = 0; i < MouseMaxMice; i++)
	{
		mouseAddresses[i] = 0;
		mice[i] = NULL;
	}
	HidUsageAttach[DesktopMouse] = MouseAttach;
}

u32 MouseIndex(u32 address) {
	if (address == 0) return 0xffffffff;

	for (u32 i = 0; i < mouseCount; i++)
		if (mouseAddresses[i] == address)
			return i;

	return 0xffffffff;
}

static u32 MouseGetAddress(u32 index) {
	if (index > mouseCount) return 0;

	for (u32 i = 0; index >= 0 && i < MouseMaxMice; i++) {
		if (mouseAddresses[i] != 0)
			if (index-- == 0)
				return mouseAddresses[i];
	}

	return 0;
}

static void MouseDetached(struct UsbDevice *device) {
	struct MouseDevice *data;
	
	data = (struct MouseDevice*)((struct HidDevice*)device->DriverData)->DriverData;
	if (data != NULL) {
		if (mouseAddresses[data->Index] == device->Number) {
			mouseAddresses[data->Index] = 0;
			mouseCount--;
			mice[data->Index] = NULL;
		}
	}
}

static void MouseDeallocate(struct UsbDevice *device) {
	struct MouseDevice *data;
	
	data = (struct MouseDevice*)((struct HidDevice*)device->DriverData)->DriverData;
	if (data != NULL) {
		MemoryDeallocate(data);
		((struct HidDevice*)device->DriverData)->DriverData = NULL;
	}
	((struct HidDevice*)device->DriverData)->HidDeallocate = NULL;
	((struct HidDevice*)device->DriverData)->HidDetached = NULL;
}

static Result MouseAttach(struct UsbDevice *device, u32 interface) {
	u32 mouseNumber;
	struct HidDevice *hidData;
	struct MouseDevice *data;
	struct HidParserResult *parse;

	if ((MouseMaxMice & 3) != 0) {
		LOG("MOUSE: Warning! MouseMaxMice not a multiple of 4. The driver wasn't built for this!\n");
	}
	if (mouseCount == MouseMaxMice) {
		LOGF("MOUSE: %s not connected. Too many mice connected (%d/%d). Change MouseMaxMice in device.mouse.c to allow more.\n", UsbGetDescription(device), mouseCount, MouseMaxMice);
		return ErrorIncompatible;
	}
	
	hidData = (struct HidDevice*)device->DriverData;
	if (hidData->Header.DeviceDriver != DeviceDriverHid) {
		LOGF("MOUSE: %s isn't a HID device. The mouse driver is built upon the HID driver.\n", UsbGetDescription(device));
		return ErrorIncompatible;
	}

	parse = hidData->ParserResult;
	if (parse->Application.Page != GenericDesktopControl ||
		parse->Application.Desktop != DesktopMouse) {
		LOGF("MOUSE: %s doesn't seem to be a mouse...\n", UsbGetDescription(device));
		return ErrorIncompatible;
	}
	if (parse->ReportCount < 1) {
		LOGF("MOUSE: %s doesn't have enough outputs to be a mouse.\n", UsbGetDescription(device));
		return ErrorIncompatible;
	}
	hidData->HidDetached = MouseDetached;
	hidData->HidDeallocate = MouseDeallocate;
	
	if ((hidData->DriverData = MemoryAllocate(sizeof(struct MouseDevice))) == NULL) {
		LOGF("MOUSE: Not enough memory to allocate mouse %s.\n", UsbGetDescription(device));
		return ErrorMemory;
	}
	data = (struct MouseDevice*)hidData->DriverData;
	data->Header.DeviceDriver = DeviceDriverMouse;
	data->Header.DataSize = sizeof(struct MouseDevice);
	data->Index = mouseNumber = 0xffffffff;
	for (u32 i = 0; i < MouseMaxMice; i++) {
		if (mouseAddresses[i] == 0) {
			data->Index = mouseNumber = i;
			mouseAddresses[i] = device->Number;
			mouseCount++;
			break;
		}
	}

	if (mouseNumber == 0xffffffff) {
		LOG("MOUSE: PANIC! Driver in inconsistent state! MouseCount is inaccurate.\n");
		MouseDeallocate(device);
		return ErrorGeneral;
	}

	mice[mouseNumber] = device;

	for (u32 i = 0; i < parse->ReportCount; i++) {
		if (parse->Report[i]->Type == Input 
//		    && data->MouseReport == NULL
		) {
			LOG_DEBUGF("MOUSE: Output report %d. %d fields.\n", i, parse->Report[i]->FieldCount);
			data->MouseReport = parse->Report[i];
		} 
	}

	data->mouseX = 0;
	data->mouseY = 0;
	data->wheel = 0;
	data->buttonState = 0;

	LOG_DEBUGF("MOUSE: New mouse assigned %d!\n", device->Number);

	return OK;
}

static u32 MouseCount() {
	return mouseCount;
}

static Result MousePoll(u32 mouseAddress) {
	u32 mouseNumber;
	Result result;
	struct MouseDevice *data;
	
	mouseNumber = MouseIndex(mouseAddress);	
	if (mouseNumber == 0xffffffff) return ErrorDisconnected;
	data = (struct MouseDevice*)((struct HidDevice*)mice[mouseNumber]->DriverData)->DriverData;
	if ((result = HidReadDevice(mice[mouseNumber], data->MouseReport->Index)) != OK) {
		if (result != ErrorDisconnected)
			LOGF("MOUSE: Could not get mouse report from %s.\n", UsbGetDescription(mice[mouseNumber]));
		return result;
	}

	struct HidParserReport *MouseReport = data->MouseReport;
	if (MouseReport->Type == Input) {
		// XXX: I'm sure I should be using HidGetFieldValue()
		// But this was so terribly easy.
		u8 *ReportBuffer = MouseReport->ReportBuffer;
		data->buttonState = (u8)ReportBuffer[0];
		data->mouseX += (s8)ReportBuffer[1];
		data->mouseY += (s8)ReportBuffer[2];
		data->wheel += (s8)ReportBuffer[3];
		if (data->mouseX < 0) {
			data->mouseX = 0;
		}
		if (data->mouseY < 0) {
			data->mouseY = 0;
		}
	}

	return OK;
}

static s16 MouseGetPositionX(u32 mouseAddress) {
	u32 mouseNumber;
	struct MouseDevice *data;
	
	mouseNumber = MouseIndex(mouseAddress);
	if (mouseNumber == 0xffffffff) return 0;
	data = (struct MouseDevice*)((struct HidDevice*)mice[mouseNumber]->DriverData)->DriverData;
	return data->mouseX;
}

static s16 MouseGetPositionY(u32 mouseAddress) {
	u32 mouseNumber;
	struct MouseDevice *data;
	
	mouseNumber = MouseIndex(mouseAddress);
	if (mouseNumber == 0xffffffff) return 0;
	data = (struct MouseDevice*)((struct HidDevice*)mice[mouseNumber]->DriverData)->DriverData;
	return data->mouseY;
}

static s16 MouseGetWheel(u32 mouseAddress) {
	u32 mouseNumber;
	struct MouseDevice *data;
	
	mouseNumber = MouseIndex(mouseAddress);
	if (mouseNumber == 0xffffffff) return 0;
	data = (struct MouseDevice*)((struct HidDevice*)mice[mouseNumber]->DriverData)->DriverData;
	return data->wheel;
}

static u32 MouseGetPosition(u32 mouseAddress) {
	u32 mouseNumber;
	struct MouseDevice *data;
	
	mouseNumber = MouseIndex(mouseAddress);
	if (mouseNumber == 0xffffffff) return 0;
	data = (struct MouseDevice*)((struct HidDevice*)mice[mouseNumber]->DriverData)->DriverData;
	return (data->mouseX << 16) | (data->mouseY & 0xFFFF);  /* x is high 16 bits; y is low 16 bits */
}

static u8 MouseGetButtons(u32 mouseAddress) {
	u32 mouseNumber;
	struct MouseDevice *data;
	
	mouseNumber = MouseIndex(mouseAddress);
	if (mouseNumber == 0xffffffff) return 0;
	data = (struct MouseDevice*)((struct HidDevice*)mice[mouseNumber]->DriverData)->DriverData;
	return data->buttonState;
}

static bool MouseGetButtonIsPressed(u32 mouseAddress, enum MouseDeviceButton 
    button) {
	u32 mouseNumber;
	struct MouseDevice *data;
	
	mouseNumber = MouseIndex(mouseAddress);
	if (mouseNumber == 0xffffffff) return 0;
	data = (struct MouseDevice*)((struct HidDevice*)mice[mouseNumber]->DriverData)->DriverData;

	switch (button) {
		case MouseDeviceButtonLeft:
			return (data->buttonState & 0x01);
		case MouseDeviceButtonRight:
			return (data->buttonState & 0x02);
		case MouseDeviceButtonMiddle:
			return (data->buttonState & 0x04);
		case MouseDeviceButtonSide:
			return (data->buttonState & 0x08);
		case MouseDeviceButtonExtra:
			return (data->buttonState & 0x10);
	}
	return false;
}
/******************************************************************************
*	hcd/dwc/designware20.c
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	hcd/dwc/designware20.c contains code to control the DesignWare� Hi-Speed
*	USB 2.0 On-The-Go (HS OTG) Controller.
*
*	THIS SOFTWARE IS NOT AFFILIATED WITH NOR ENDORSED BY SYNOPSYS IP.
******************************************************************************/

#ifndef HCD_DESIGNWARE_BASE
#error Missing required definition HCD_DESIGNWARE_BASE. Should be of the form ((void*)0xhhhhhhhh). Should be defined after HCD_DESIGNWARE_20 in the platform.
#endif

static void DwcLoad() {
	LOG_DEBUG("CSUD: DesignWare Hi-Speed USB 2.0 On-The-Go (HS OTG) Controller driver version 0.1\n"); 
}

static void WriteThroughReg(volatile const void* reg) {
	WriteThroughRegMask(reg, 0);
}
static void WriteThroughRegMask(volatile const void* reg, u32 maskOr) {
	if ((u32)reg - (u32)Core < sizeof(struct CoreGlobalRegs)) {
		maskOr |= 0xffffffff;
		*(u32*)((u32)reg - (u32)Core + (u32)CorePhysical) = *((u32*)reg) & maskOr;
	} else if ((u32)reg - (u32)Host < sizeof(struct HostGlobalRegs)) {
		switch ((u32)reg - (u32)Host) {
		case 0x40: // Host->Port
			maskOr |= 0x1f140;
			break;
		default:
			maskOr |= 0xffffffff;
			break;
		}
		*(u32*)((u32)reg - (u32)Host + (u32)HostPhysical) = *((u32*)reg) & maskOr;
	} else if ((u32)reg == (u32)Power) {
		maskOr |= 0xffffffff;
		*(u32*)PowerPhysical = *(u32*)Power & maskOr;
	}
}
static void ReadBackReg(volatile const void* reg) {
	if ((u32)reg - (u32)Core < sizeof(struct CoreGlobalRegs)) {
		switch ((u32)reg - (u32)Core) {
		case 0x44: // Core->Hardware
			*((u32*)reg + 0) = *((u32*)((u32)reg - (u32)Core + (u32)CorePhysical) + 0);
			*((u32*)reg + 1) = *((u32*)((u32)reg - (u32)Core + (u32)CorePhysical) + 1);
			*((u32*)reg + 2) = *((u32*)((u32)reg - (u32)Core + (u32)CorePhysical) + 2);
			*((u32*)reg + 3) = *((u32*)((u32)reg - (u32)Core + (u32)CorePhysical) + 3);
			break;
		default:
			*(u32*)reg = *(u32*)((u32)reg - (u32)Core + (u32)CorePhysical);
		}
	} else if ((u32)reg - (u32)Host < sizeof(struct HostGlobalRegs)) {
		*(u32*)reg = *(u32*)((u32)reg - (u32)Host + (u32)HostPhysical);
	} else if ((u32)reg == (u32)Power) {
		*(u32*)Power = *(u32*)PowerPhysical;
	}
}
static void ClearReg(volatile const void* reg) {
	if ((u32)reg - (u32)Core < sizeof(struct CoreGlobalRegs)) {
		switch ((u32)reg - (u32)Core) {
		case 0x44: // Core->Hardware
			*((u32*)reg + 0) = 0;
			*((u32*)reg + 1) = 0;
			*((u32*)reg + 2) = 0;
			*((u32*)reg + 3) = 0;
			break;
		default:
			*(u32*)reg = 0;
		}
	} else if ((u32)reg - (u32)Host < sizeof(struct HostGlobalRegs)) {
		*(u32*)reg = 0;
	} else if ((u32)reg == (u32)Power) {
		*(u32*)Power = 0;
	}
}
static void SetReg(volatile const void* reg) {
	u32 value;
	if ((u32)reg - (u32)Core < sizeof(struct CoreGlobalRegs)) {
		value = 0xffffffff;
		switch ((u32)reg - (u32)Core) {
		case 0x44: // Core->Hardware
			*((u32*)reg + 0) = value;
			*((u32*)reg + 1) = value;
			*((u32*)reg + 2) = value;
			*((u32*)reg + 3) = value;
			break;
		default:
			*(u32*)reg = value;
		}
	} else if ((u32)reg - (u32)Host < sizeof(struct HostGlobalRegs)) {
		if ((u32)reg - (u32)Host > 0x100 && (u32)reg - (u32)Host < 0x300) {
			switch (((u32)reg - (u32)Host) & 0x1f) {
			case 0x8:
				value = 0x3fff;
				break;
			default:
				value = 0xffffffff;
				break;
			}
		} else
			value = 0xffffffff;

		*(u32*)reg = value;
	} else if ((u32)reg == (u32)Power) {
		value = 0xffffffff;
		*(u32*)Power = value;
	}
}


/** 
	\brief Triggers the core soft reset.

	Raises the core soft reset signal high, and then waits for the core to 
	signal that it is ready again.
*/
static Result HcdReset() {
	u32 count = 0;
	
	do {
		ReadBackReg(&Core->Reset);
		if (count++ >= 0x100000) {
			LOG("HCD: Device Hang!\n");
			return ErrorDevice;
		}
	} while (Core->Reset.AhbMasterIdle == false);

	Core->Reset.CoreSoft = true;
	WriteThroughReg(&Core->Reset);
	count = 0;
	
	do {
		ReadBackReg(&Core->Reset);
		if (count++ >= 0x100000) {
			LOG("HCD: Device Hang!\n");
			return ErrorDevice;
		}
	} while (Core->Reset.CoreSoft == true || Core->Reset.AhbMasterIdle == false);

	return OK;
}

/** 
	\brief Triggers the fifo flush for a given fifo.

	Raises the core fifo flush signal high, and then waits for the core to 
	signal that it is ready again.
*/
static Result HcdTransmitFifoFlush(enum CoreFifoFlush fifo) {
	u32 count = 0;
	
	if (fifo == FlushAll)
		LOG_DEBUG("HCD: TXFlush(All)\n");
	else if (fifo == FlushNonPeriodic)
		LOG_DEBUG("HCD: TXFlush(NP)\n");
	else
		LOG_DEBUGF("HCD: TXFlush(P%u)\n", fifo);

	ClearReg(&Core->Reset);
	Core->Reset.TransmitFifoFlushNumber = fifo;
	Core->Reset.TransmitFifoFlush = true;
	WriteThroughReg(&Core->Reset);
	count = 0;
	
	do {
		ReadBackReg(&Core->Reset);
		if (count++ >= 0x100000) {
			LOG("HCD: Device Hang!\n");
			return ErrorDevice;
		}
	} while (Core->Reset.TransmitFifoFlush == true);

	return OK;
}

/** 
	\brief Triggers the receive fifo flush for a given fifo.

	Raises the core receive fifo flush signal high, and then waits for the core to 
	signal that it is ready again.
*/
static Result HcdReceiveFifoFlush() {
	u32 count = 0;
	
	LOG_DEBUG("HCD: RXFlush(All)\n");
	
	ClearReg(&Core->Reset);
	Core->Reset.ReceiveFifoFlush = true;
	WriteThroughReg(&Core->Reset);
	count = 0;
	
	do {
		ReadBackReg(&Core->Reset);
		if (count++ >= 0x100000) {
			LOG("HCD: Device Hang!\n");
			return ErrorDevice;
		}
	} while (Core->Reset.ReceiveFifoFlush == true);

	return OK;
}

/**
	\brief Prepares a channel to communicated with a device.

	Prepares a channel to communicated with the device specified in pipe.
*/
static Result HcdPrepareChannel(struct UsbDevice *device, u8 channel,
	u32 length, enum PacketId type, struct UsbPipeAddress *pipe) {
	if (channel > Core->Hardware.HostChannelCount) {
		LOGF("HCD: Channel %d is not available on this host.\n", channel);
		return ErrorArgument;
	}

	// Clear all existing interrupts.
	SetReg(&Host->Channel[channel].Interrupt);
	WriteThroughReg(&Host->Channel[channel].Interrupt);

	// Program the channel.
	ClearReg(&Host->Channel[channel].Characteristic);
	Host->Channel[channel].Characteristic.DeviceAddress = pipe->Device;
	Host->Channel[channel].Characteristic.EndPointNumber = pipe->EndPoint;
	Host->Channel[channel].Characteristic.EndPointDirection = pipe->Direction;
	Host->Channel[channel].Characteristic.LowSpeed = pipe->Speed == Low ? true : false;
	Host->Channel[channel].Characteristic.Type = pipe->Type;
	Host->Channel[channel].Characteristic.MaximumPacketSize = SizeToNumber(pipe->MaxSize);
	Host->Channel[channel].Characteristic.Enable = false;
	Host->Channel[channel].Characteristic.Disable = false;
	WriteThroughReg(&Host->Channel[channel].Characteristic);

	// Clear split control.
	ClearReg(&Host->Channel[channel].SplitControl);
	if (pipe->Speed != High) {
		Host->Channel[channel].SplitControl.SplitEnable = true;
		Host->Channel[channel].SplitControl.HubAddress = device->Parent->Number;
		Host->Channel[channel].SplitControl.PortAddress = device->PortNumber;			
	}
	WriteThroughReg(&Host->Channel[channel].SplitControl);

	ClearReg(&Host->Channel[channel].TransferSize);
	Host->Channel[channel].TransferSize.TransferSize = length;
	if (pipe->Speed == Low)
		Host->Channel[channel].TransferSize.PacketCount = (length + 7) / 8;
	else
		Host->Channel[channel].TransferSize.PacketCount = (length + Host->Channel[channel].Characteristic.MaximumPacketSize -  1) / Host->Channel[channel].Characteristic.MaximumPacketSize;
	if (Host->Channel[channel].TransferSize.PacketCount == 0)
		Host->Channel[channel].TransferSize.PacketCount = 1;
	Host->Channel[channel].TransferSize.PacketId = type;
	WriteThroughReg(&Host->Channel[channel].TransferSize);
	
	return OK;
}

static void HcdTransmitChannel(u8 channel, void* buffer) {	
    ReadBackReg(&Host->Channel[channel].SplitControl);
	Host->Channel[channel].SplitControl.CompleteSplit = false;
	WriteThroughReg(&Host->Channel[channel].SplitControl);

	if (((u32)buffer & 3) != 0)
		LOG_DEBUGF("HCD: Transfer buffer %#x is not DWORD aligned. Ignored, but dangerous.\n", buffer);
	Host->Channel[channel].DmaAddress = buffer;
	WriteThroughReg(&Host->Channel[channel].DmaAddress);

	ReadBackReg(&Host->Channel[channel].Characteristic);
	Host->Channel[channel].Characteristic.PacketsPerFrame = 1;
	Host->Channel[channel].Characteristic.Enable = true;
	Host->Channel[channel].Characteristic.Disable = false;
	WriteThroughReg(&Host->Channel[channel].Characteristic);	
}

static Result HcdChannelInterruptToError(struct UsbDevice *device, struct 
    ChannelInterrupts interrupts, bool isComplete) {
	Result result;

	result = OK;
	if (interrupts.AhbError) {
		device->Error = AhbError;
		LOG("HCD: AHB error in transfer.\n");
		return ErrorDevice;
	}
	if (interrupts.Stall) {
		device->Error =  Stall;
		LOG("HCD: Stall error in transfer.\n");
		return ErrorDevice;
	}
	if (interrupts.NegativeAcknowledgement) {
		device->Error =  NoAcknowledge;
		LOG("HCD: NAK error in transfer.\n");
		return ErrorDevice;
	}
	if (!interrupts.Acknowledgement) {
		LOG("HCD: Transfer was not acknowledged.\n");
		result = ErrorTimeout;
	}
	if (interrupts.NotYet) {
		device->Error =  NotYetError;
		LOG("HCD: Not yet error in transfer.\n");
		return ErrorDevice;
	}
	if (interrupts.BabbleError) {
		device->Error =  Babble;
		LOG("HCD: Babble error in transfer.\n");
		return ErrorDevice;
	}
	if (interrupts.FrameOverrun) {
		device->Error =  BufferError;
		LOG("HCD: Frame overrun in transfer.\n");
		return ErrorDevice;
	}
	if (interrupts.DataToggleError) {
		device->Error =  BitError;
		LOG("HCD: Data toggle error in transfer.\n");
		return ErrorDevice;
	}
	if (interrupts.TransactionError) {
		device->Error =  ConnectionError;
		LOG("HCD: Transaction error in transfer.\n");
		return ErrorDevice;
	}
	if (!interrupts.TransferComplete && isComplete) {
		LOG("HCD: Transfer did not complete.\n");
		result = ErrorTimeout;
	}
	return result;
}

static Result HcdChannelSendWaitOne(struct UsbDevice *device, struct 
    UsbPipeAddress *pipe, u8 channel, void* buffer, u32 bufferLength, u32 
bufferOffset,
	struct UsbDeviceRequest *request) {
	Result result;
	u32 timeout, tries, globalTries, actualTries;
	
	for (globalTries = 0, actualTries = 0; globalTries < 3 && actualTries < 10; globalTries++, actualTries++) {
		SetReg(&Host->Channel[channel].Interrupt);
		WriteThroughReg(&Host->Channel[channel].Interrupt);
		ReadBackReg(&Host->Channel[channel].TransferSize);
		ReadBackReg(&Host->Channel[channel].SplitControl);
						
		HcdTransmitChannel(channel, (u8*)buffer + bufferOffset);

		timeout = 0;
		do {
			if (timeout++ == RequestTimeout) {
				LOGF("HCD: Request to %s has timed out.\n", UsbGetDescription(device));
				device->Error = ConnectionError;
				return ErrorTimeout;
			}
			ReadBackReg(&Host->Channel[channel].Interrupt);
			if (!Host->Channel[channel].Interrupt.Halt) MicroDelay(10);
			else break;
		} while (true);
		ReadBackReg(&Host->Channel[channel].TransferSize);
		
		if (Host->Channel[channel].SplitControl.SplitEnable) {
			if (Host->Channel[channel].Interrupt.Acknowledgement) {
				for (tries = 0; tries < 3; tries++) {
					SetReg(&Host->Channel[channel].Interrupt);
					WriteThroughReg(&Host->Channel[channel].Interrupt);

					ReadBackReg(&Host->Channel[channel].SplitControl);
					Host->Channel[channel].SplitControl.CompleteSplit = true;
					WriteThroughReg(&Host->Channel[channel].SplitControl);
					
					Host->Channel[channel].Characteristic.Enable = true;
					Host->Channel[channel].Characteristic.Disable = false;
					WriteThroughReg(&Host->Channel[channel].Characteristic);

					timeout = 0;
					do {
						if (timeout++ == RequestTimeout) {
							LOGF("HCD: Request split completion to %s has timed out.\n", UsbGetDescription(device));
							device->Error = ConnectionError;
							return ErrorTimeout;
						}
						ReadBackReg(&Host->Channel[channel].Interrupt);
						if (!Host->Channel[channel].Interrupt.Halt) MicroDelay(100);
						else break;
					} while (true);
					if (!Host->Channel[channel].Interrupt.NotYet) break;
				}

				if (tries == 3) {
					MicroDelay(25000);
					continue;
				} else if (Host->Channel[channel].Interrupt.NegativeAcknowledgement) {
					globalTries--;
					MicroDelay(25000);
					continue;
				} else if (Host->Channel[channel].Interrupt.TransactionError) {
					MicroDelay(25000);
					continue;
				}
	
				if ((result = HcdChannelInterruptToError(device, Host->Channel[channel].Interrupt, false)) != OK) {
					LOG_DEBUGF("HCD: Control message to %#x: %02x%02x%02x%02x %02x%02x%02x%02x.\n", *(u32*)pipe, 
						((u8*)request)[0], ((u8*)request)[1], ((u8*)request)[2], ((u8*)request)[3],
						((u8*)request)[4], ((u8*)request)[5], ((u8*)request)[6], ((u8*)request)[7]);
					LOGF("HCD: Request split completion to %s failed.\n", UsbGetDescription(device));
					return result;
				}
			} else if (Host->Channel[channel].Interrupt.NegativeAcknowledgement) {
				globalTries--;
				MicroDelay(25000);
				continue;
			} else if (Host->Channel[channel].Interrupt.TransactionError) {
				MicroDelay(25000);
				continue;
			}				
		} else {				
			if ((result = HcdChannelInterruptToError(device, Host->Channel[channel].Interrupt, !Host->Channel[channel].SplitControl.SplitEnable)) != OK) {
				LOG_DEBUGF("HCD: Control message to %#x: %02x%02x%02x%02x %02x%02x%02x%02x.\n", *(u32*)pipe, 
					((u8*)request)[0], ((u8*)request)[1], ((u8*)request)[2], ((u8*)request)[3],
					((u8*)request)[4], ((u8*)request)[5], ((u8*)request)[6], ((u8*)request)[7]);
				LOGF("HCD: Request to %s failed.\n", UsbGetDescription(device));
				return ErrorRetry;
			}
		}

		break;
	}

	if (globalTries == 3 || actualTries == 10) {
		LOGF("HCD: Request to %s has failed 3 times.\n", UsbGetDescription(device));
		if ((result = HcdChannelInterruptToError(device, Host->Channel[channel].Interrupt, !Host->Channel[channel].SplitControl.SplitEnable)) != OK) {
			LOG_DEBUGF("HCD: Control message to %#x: %02x%02x%02x%02x %02x%02x%02x%02x.\n", *(u32*)pipe, 
				((u8*)request)[0], ((u8*)request)[1], ((u8*)request)[2], ((u8*)request)[3],
				((u8*)request)[4], ((u8*)request)[5], ((u8*)request)[6], ((u8*)request)[7]);
			LOGF("HCD: Request to %s failed.\n", UsbGetDescription(device));
			return result;
		}
		device->Error = ConnectionError;
		return ErrorTimeout;
	}

	return OK;
}

static Result HcdChannelSendWait(struct UsbDevice *device, struct 
    UsbPipeAddress *pipe, u8 channel, void* buffer, u32 bufferLength, struct 
UsbDeviceRequest *request, enum PacketId packetId) {
	Result result;
	u32 packets, transfer, tries;
	
	tries = 0;
retry:
	if (tries++ == 3) {
		LOGF("HCD: Failed to send to %s after 3 attempts.\n", UsbGetDescription(device));
		return ErrorTimeout;
	} 

	if ((result = HcdPrepareChannel(device, channel, bufferLength, packetId, pipe)) != OK) {		
		device->Error = ConnectionError;
		LOGF("HCD: Could not prepare data channel to %s.\n", UsbGetDescription(device));
		return result;
	}
		
	transfer = 0;
	do {
		packets = Host->Channel[channel].TransferSize.PacketCount;
		if ((result = HcdChannelSendWaitOne(device, pipe, channel, buffer, bufferLength, transfer, request)) != OK) {
			if (result == ErrorRetry) goto retry;
			return result;
		}
		
		ReadBackReg(&Host->Channel[channel].TransferSize);		
		transfer = bufferLength - Host->Channel[channel].TransferSize.TransferSize;
		if (packets == Host->Channel[channel].TransferSize.PacketCount) break;
	} while (Host->Channel[channel].TransferSize.PacketCount > 0);

	if (packets == Host->Channel[channel].TransferSize.PacketCount) {
		device->Error = ConnectionError;
		LOGF("HCD: Transfer to %s got stuck.\n", UsbGetDescription(device));
		return ErrorDevice;
	}

	if (tries > 1) {
		LOGF("HCD: Transfer to %s succeeded on attempt %d/3.\n", UsbGetDescription(device), tries);
	}

	return OK;
}

static Result HcdSumbitControlMessage(struct UsbDevice *device, struct 
    UsbPipeAddress pipe, void* buffer, u32 bufferLength,
	struct UsbDeviceRequest *request) {
	Result result;
	struct UsbPipeAddress tempPipe;
	if (pipe.Device == RootHubDeviceNumber) {
		return HcdProcessRootHubMessage(device, pipe, buffer, bufferLength, request);
	}

	device->Error = Processing;
	device->LastTransfer = 0;
			
	// Setup
	tempPipe.Speed = pipe.Speed;
	tempPipe.Device = pipe.Device;
	tempPipe.EndPoint = pipe.EndPoint;
	tempPipe.MaxSize = pipe.MaxSize;
	tempPipe.Type = Control;
	tempPipe.Direction = Out;
	
	if ((result = HcdChannelSendWait(device, &tempPipe, 0, request, 8, request, Setup)) != OK) {		
		LOGF("HCD: Could not send SETUP to %s.\n", UsbGetDescription(device));
		return OK;
	}

	// Data
	if (buffer != NULL) {
		if (pipe.Direction == Out) {
			MemoryCopy(databuffer, buffer, bufferLength);
		}
		tempPipe.Speed = pipe.Speed;
		tempPipe.Device = pipe.Device;
		tempPipe.EndPoint = pipe.EndPoint;
		tempPipe.MaxSize = pipe.MaxSize;
		tempPipe.Type = Control;
		tempPipe.Direction = pipe.Direction;
		
		if ((result = HcdChannelSendWait(device, &tempPipe, 0, databuffer, bufferLength, request, Data1)) != OK) {		
			LOGF("HCD: Could not send DATA to %s.\n", UsbGetDescription(device));
			return OK;
		}
						
		ReadBackReg(&Host->Channel[0].TransferSize);
		if (pipe.Direction == In) {
			if (Host->Channel[0].TransferSize.TransferSize <= bufferLength)
				device->LastTransfer = bufferLength - Host->Channel[0].TransferSize.TransferSize;
			else{
				LOG_DEBUGF("HCD: Weird transfer.. %d/%d bytes received.\n", Host->Channel[0].TransferSize.TransferSize, bufferLength);
				LOG_DEBUGF("HCD: Message %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x ...\n", 
					((u8*)databuffer)[0x0],((u8*)databuffer)[0x1],((u8*)databuffer)[0x2],((u8*)databuffer)[0x3],
					((u8*)databuffer)[0x4],((u8*)databuffer)[0x5],((u8*)databuffer)[0x6],((u8*)databuffer)[0x7],
					((u8*)databuffer)[0x8],((u8*)databuffer)[0x9],((u8*)databuffer)[0xa],((u8*)databuffer)[0xb],
					((u8*)databuffer)[0xc],((u8*)databuffer)[0xd],((u8*)databuffer)[0xe],((u8*)databuffer)[0xf]);
				device->LastTransfer = bufferLength;
			}
			MemoryCopy(buffer, databuffer, device->LastTransfer);
		}
		else {
			device->LastTransfer = bufferLength;
		}
	}

	// Status
	tempPipe.Speed = pipe.Speed;
	tempPipe.Device = pipe.Device;
	tempPipe.EndPoint = pipe.EndPoint;
	tempPipe.MaxSize = pipe.MaxSize;
	tempPipe.Type = Control;
	tempPipe.Direction = ((bufferLength == 0) || pipe.Direction == Out) ? In : Out;
	
	if ((result = HcdChannelSendWait(device, &tempPipe, 0, databuffer, 0, request, Data1)) != OK) {		
		LOGF("HCD: Could not send STATUS to %s.\n", UsbGetDescription(device));
		return OK;
	}

	ReadBackReg(&Host->Channel[0].TransferSize);
	if (Host->Channel[0].TransferSize.TransferSize != 0)
		LOG_DEBUGF("HCD: Warning non zero status transfer! %d.\n", Host->Channel[0].TransferSize.TransferSize);

	device->Error = NoError;

	return OK;
}
	
static Result HcdInitialise() {	volatile Result result;

	if (sizeof(struct CoreGlobalRegs) != 0x400 || sizeof(struct HostGlobalRegs) != 0x400 || sizeof(struct PowerReg) != 0x4) {
		LOGF("HCD: Incorrectly compiled driver. HostGlobalRegs: %#x (0x400), CoreGlobalRegs: %#x (0x400), PowerReg: %#x (0x4).\n", 
			sizeof(struct HostGlobalRegs), sizeof(struct CoreGlobalRegs), sizeof(struct PowerReg));
		return ErrorCompiler; // Correct packing settings are required.
	}
	LOG_DEBUG("HCD: Reserving memory.\n");
	CorePhysical = MemoryReserve(sizeof(struct CoreGlobalRegs), HCD_DESIGNWARE_BASE);
	Core = MemoryAllocate(sizeof(struct CoreGlobalRegs));

	HostPhysical = MemoryReserve(sizeof(struct HostGlobalRegs), (void*)((u8*)HCD_DESIGNWARE_BASE + 0x400));
	Host = MemoryAllocate(sizeof(struct HostGlobalRegs));
	
	PowerPhysical = MemoryReserve(sizeof(struct PowerReg), (void*)((u8*)HCD_DESIGNWARE_BASE + 0xe00));
	Power = MemoryAllocate(sizeof(struct PowerReg));

#ifdef BROADCOM_2835
	ReadBackReg(&Core->VendorId);
	ReadBackReg(&Core->UserId);
	if ((Core->VendorId & 0xfffff000) != 0x4f542000) { // 'OT'2 
		LOGF("HCD: Hardware: %c%c%x.%x%x%x (BCM%.5x). Driver incompatible. Expected OT2.xxx (BCM2708x).\n",
			(Core->VendorId >> 24) & 0xff, (Core->VendorId >> 16) & 0xff,
			(Core->VendorId >> 12) & 0xf, (Core->VendorId >> 8) & 0xf,
			(Core->VendorId >> 4) & 0xf, (Core->VendorId >> 0) & 0xf, 
			(Core->UserId >> 12) & 0xFFFFF);
		result = ErrorIncompatible;
		goto deallocate;
	}
	else {
		LOGF("HCD: Hardware: %c%c%x.%x%x%x (BCM%.5x).\n",
			(Core->VendorId >> 24) & 0xff, (Core->VendorId >> 16) & 0xff,
			(Core->VendorId >> 12) & 0xf, (Core->VendorId >> 8) & 0xf,
			(Core->VendorId >> 4) & 0xf, (Core->VendorId >> 0) & 0xf, 
			(Core->UserId >> 12) & 0xFFFFF);
	}
#else
	if ((Core->VendorId & 0xfffff000) != 0x4f542000) { // 'OT'2 
		LOGF("HCD: Hardware: %c%c%x.%x%x%x. Driver incompatible. Expected OT2.xxx.\n",
			(Core->VendorId >> 24) & 0xff, (Core->VendorId >> 16) & 0xff,
			(Core->VendorId >> 12) & 0xf, (Core->VendorId >> 8) & 0xf,
			(Core->VendorId >> 4) & 0xf, (Core->VendorId >> 0) & 0xf);
		return ErrorIncompatible;
	}
	else {
		LOGF("HCD: Hardware: %c%c%x.%x%x%x.\n",
			(Core->VendorId >> 24) & 0xff, (Core->VendorId >> 16) & 0xff,
			(Core->VendorId >> 12) & 0xf, (Core->VendorId >> 8) & 0xf,
			(Core->VendorId >> 4) & 0xf, (Core->VendorId >> 0) & 0xf);
	}
#endif

	ReadBackReg(&Core->Hardware);
	if (Core->Hardware.Architecture != InternalDma) {
		LOG("HCD: Host architecture is not Internal DMA. Driver incompatible.\n");
		result = ErrorIncompatible;
		goto deallocate;
	}
	LOG_DEBUG("HCD: Internal DMA mode.\n");
	if (Core->Hardware.HighSpeedPhysical == NotSupported) {
		LOG("HCD: High speed physical unsupported. Driver incompatible.\n");
		result = ErrorIncompatible;
		goto deallocate;
	}
	LOG_DEBUGF("HCD: Hardware configuration: %08x %08x %08x %08x\n", *(u32*)&Core->Hardware, *((u32*)&Core->Hardware + 1), *((u32*)&Core->Hardware + 2), *((u32*)&Core->Hardware + 3));
	ReadBackReg(&Host->Config);
	LOG_DEBUGF("HCD: Host configuration: %08x\n", *(u32*)&Host->Config);
	
	LOG_DEBUG("HCD: Disabling interrupts.\n");
	ReadBackReg(&Core->Ahb);
	Core->Ahb.InterruptEnable = false;
	ClearReg(&Core->InterruptMask);
	WriteThroughReg(&Core->InterruptMask);
	WriteThroughReg(&Core->Ahb);
	
	LOG_DEBUG("HCD: Powering USB on.\n");
	if ((result = PowerOnUsb()) != OK) {
		LOG("HCD: Failed to power on USB Host Controller.\n");
		result = ErrorIncompatible;
		goto deallocate;
	}
	
	LOG_DEBUG("HCD: Load completed.\n");

	return OK;
deallocate:
	if (Core != NULL) MemoryDeallocate((void *)Core);
	if (Host != NULL) MemoryDeallocate((void *)Host);
	if (Power != NULL) MemoryDeallocate((void *)Power);
	return result;
}

static Result HcdStart() {	Result result;
	u32 timeout;

	LOG_DEBUG("HCD: Start core.\n");
	if (Core == NULL) {
		LOG("HCD: HCD uninitialised. Cannot be started.\n");
		return ErrorDevice;
	}

	if ((databuffer = MemoryAllocate(1024)) == NULL)
		return ErrorMemory;

	ReadBackReg(&Core->Usb);
	Core->Usb.UlpiDriveExternalVbus = 0;
	Core->Usb.TsDlinePulseEnable = 0;
	WriteThroughReg(&Core->Usb);
	
	LOG_DEBUG("HCD: Master reset.\n");
	if ((result = HcdReset()) != OK) {
		goto deallocate;
	}
	
	if (!PhyInitialised) {
		LOG_DEBUG("HCD: One time phy initialisation.\n");
		PhyInitialised = true;

		Core->Usb.ModeSelect = UTMI;
		LOG_DEBUG("HCD: Interface: UTMI+.\n");
		Core->Usb.PhyInterface = false;

		WriteThroughReg(&Core->Usb);
		HcdReset();
	}

	ReadBackReg(&Core->Usb);
	if (Core->Hardware.HighSpeedPhysical == Ulpi
		&& Core->Hardware.FullSpeedPhysical == Dedicated) {
		LOG_DEBUG("HCD: ULPI FSLS configuration: enabled.\n");
		Core->Usb.UlpiFsls = true;
		Core->Usb.ulpi_clk_sus_m = true;
	} else {
		LOG_DEBUG("HCD: ULPI FSLS configuration: disabled.\n");
		Core->Usb.UlpiFsls = false;
		Core->Usb.ulpi_clk_sus_m = false;
	}
	WriteThroughReg(&Core->Usb);

	LOG_DEBUG("HCD: DMA configuration: enabled.\n");
	ReadBackReg(&Core->Ahb);
	Core->Ahb.DmaEnable = true;
	Core->Ahb.DmaRemainderMode = Incremental;
	WriteThroughReg(&Core->Ahb);
	
	ReadBackReg(&Core->Usb);
	switch (Core->Hardware.OperatingMode) {
	case HNP_SRP_CAPABLE:
		LOG_DEBUG("HCD: HNP/SRP configuration: HNP, SRP.\n");
		Core->Usb.HnpCapable = true;
		Core->Usb.SrpCapable = true;
		break;
	case SRP_ONLY_CAPABLE:
	case SRP_CAPABLE_DEVICE:
	case SRP_CAPABLE_HOST:
		LOG_DEBUG("HCD: HNP/SRP configuration: SRP.\n");
		Core->Usb.HnpCapable = false;
		Core->Usb.SrpCapable = true;
		break;
	case NO_HNP_SRP_CAPABLE:
	case NO_SRP_CAPABLE_DEVICE:
	case NO_SRP_CAPABLE_HOST:
		LOG_DEBUG("HCD: HNP/SRP configuration: none.\n");
		Core->Usb.HnpCapable = false;
		Core->Usb.SrpCapable = false;
		break;
	}
	WriteThroughReg(&Core->Usb);
	LOG_DEBUG("HCD: Core started.\n");
	LOG_DEBUG("HCD: Starting host.\n");

	ClearReg(Power);
	WriteThroughReg(Power);

	ReadBackReg(&Host->Config);
	if (Core->Hardware.HighSpeedPhysical == Ulpi
		&& Core->Hardware.FullSpeedPhysical == Dedicated
		&& Core->Usb.UlpiFsls) {
		LOG_DEBUG("HCD: Host clock: 48Mhz.\n");
		Host->Config.ClockRate = Clock48MHz;
	} else {
		LOG_DEBUG("HCD: Host clock: 30-60Mhz.\n");
		Host->Config.ClockRate = Clock30_60MHz;
	}
	WriteThroughReg(&Host->Config);

	ReadBackReg(&Host->Config);
	Host->Config.FslsOnly = true;
	WriteThroughReg(&Host->Config);
		
	ReadBackReg(&Host->Config);
	if (Host->Config.EnableDmaDescriptor == 
		Core->Hardware.DmaDescription &&
		(Core->VendorId & 0xfff) >= 0x90a) {
		LOG_DEBUG("HCD: DMA descriptor: enabled.\n");
	} else {
		LOG_DEBUG("HCD: DMA descriptor: disabled.\n");
	}
	WriteThroughReg(&Host->Config);
		
	LOG_DEBUGF("HCD: FIFO configuration: Total=%#x Rx=%#x NPTx=%#x PTx=%#x.\n", ReceiveFifoSize + NonPeriodicFifoSize + PeriodicFifoSize, ReceiveFifoSize, NonPeriodicFifoSize, PeriodicFifoSize);
	ReadBackReg(&Core->Receive.Size);
	Core->Receive.Size = ReceiveFifoSize;
	WriteThroughReg(&Core->Receive.Size);

	ReadBackReg(&Core->NonPeriodicFifo.Size);
	Core->NonPeriodicFifo.Size.Depth = NonPeriodicFifoSize;
	Core->NonPeriodicFifo.Size.StartAddress = ReceiveFifoSize;
	WriteThroughReg(&Core->NonPeriodicFifo.Size);

	ReadBackReg(&Core->PeriodicFifo.HostSize);
	Core->PeriodicFifo.HostSize.Depth = PeriodicFifoSize;
	Core->PeriodicFifo.HostSize.StartAddress = ReceiveFifoSize + NonPeriodicFifoSize;
	WriteThroughReg(&Core->PeriodicFifo.HostSize);

	LOG_DEBUG("HCD: Set HNP: enabled.\n");
	ReadBackReg(&Core->OtgControl);
	Core->OtgControl.HostSetHnpEnable = true;
	WriteThroughReg(&Core->OtgControl);

	if ((result = HcdTransmitFifoFlush(FlushAll)) != OK) 
		goto deallocate;
	if ((result = HcdReceiveFifoFlush()) != OK)
		goto deallocate;

	if (!Host->Config.EnableDmaDescriptor) {
		for (u32 channel = 0; channel < Core->Hardware.HostChannelCount; channel++) {
			ReadBackReg(&Host->Channel[channel].Characteristic);
			Host->Channel[channel].Characteristic.Enable = false;
			Host->Channel[channel].Characteristic.Disable = true;
			Host->Channel[channel].Characteristic.EndPointDirection = In;
			WriteThroughReg(&Host->Channel[channel].Characteristic);
			timeout = 0;
		}

		// Halt channels to put them into known state.
		for (u32 channel = 0; channel < Core->Hardware.HostChannelCount; channel++) {
			ReadBackReg(&Host->Channel[channel].Characteristic);
			Host->Channel[channel].Characteristic.Enable = true;
			Host->Channel[channel].Characteristic.Disable = true;
			Host->Channel[channel].Characteristic.EndPointDirection = In;
			WriteThroughReg(&Host->Channel[channel].Characteristic);
			timeout = 0;
			do {
				ReadBackReg(&Host->Channel[channel].Characteristic);

				if (timeout++ > 0x100000) {
					LOGF("HCD: Unable to clear halt on channel %u.\n", channel);
				}
			} while (Host->Channel[channel].Characteristic.Enable);
		}
	}

	ReadBackReg(&Host->Port);
	if (!Host->Port.Power) {
		LOG_DEBUG("HCD: Powering up port.\n");
		Host->Port.Power = true;
		WriteThroughRegMask(&Host->Port, 0x1000);
	}
	
	LOG_DEBUG("HCD: Reset port.\n");
	ReadBackReg(&Host->Port);
	Host->Port.Reset = true;
	WriteThroughRegMask(&Host->Port, 0x100);
	MicroDelay(50000);
	Host->Port.Reset = false;
	WriteThroughRegMask(&Host->Port, 0x100);
	ReadBackReg(&Host->Port);
	
	LOG_DEBUG("HCD: Successfully started.\n");
		
	return OK;
deallocate:
	MemoryDeallocate(databuffer);
	return result;
}

static Result HcdStop() {
	if (databuffer != NULL) MemoryDeallocate(databuffer);
	return OK;
}

static Result HcdDeinitialise() {
	if (Core != NULL) MemoryDeallocate((void *)Core);
	if (Host != NULL) MemoryDeallocate((void *)Host);
	if (Power != NULL) MemoryDeallocate((void *)Power);
	return OK;
}

/******************************************************************************
*	hcd/dwc/roothub.c
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	hcd/dwc/roothub.c contains code to control the DesignWare® Hi-Speed USB 2.0
*	On-The-Go (HS OTG) Controller's virtual root hub. The physical USB 
*	connection to the computer is treated as a virtual 1 port USB hub for 
*	simplicity, allowing the USBD to control it directly with a Hub driver.
*
*	THIS SOFTWARE IS NOT AFFILIATED WITH NOR ENDORSED BY SYNOPSYS IP.
******************************************************************************/

static struct UsbDeviceDescriptor DeviceDescriptor = {
	.DescriptorLength = 0x12,
	.DescriptorType = Device,
	.UsbVersion = 0x0200,
	.Class = DeviceClassHub,
	.SubClass = 0,
	.Protocol = 0,
	.MaxPacketSize0 = 8,
	.VendorId = 0,
	.ProductId = 0,
	.Version = 0x0100,
	.Manufacturer = 0,
	.Product = 1,
	.SerialNumber = 0,
	.ConfigurationCount = 1,
};

__attribute__ ((__packed__))
struct XXX1 {
    struct UsbConfigurationDescriptor Configuration;
	struct UsbInterfaceDescriptor Interface;
	struct UsbEndpointDescriptor Endpoint;
};

static struct XXX1 ConfigurationDescriptor = {
	.Configuration = {
		.DescriptorLength = 9,
		.DescriptorType = Configuration,
		.TotalLength = 0x19,
		.InterfaceCount = 1,
		.ConfigurationValue = 1,
		.StringIndex = 0,
		.Attributes = {
			.RemoteWakeup = false,
			.SelfPowered = true,
			._reserved7 = 1,
		},
		.MaximumPower = 0,
	},
	.Interface = {
		.DescriptorLength = 9,
		.DescriptorType = Interface,
		.Number = 0,
		.AlternateSetting = 0,
		.EndpointCount = 1,
		.Class = InterfaceClassHub,
		.SubClass = 0,
		.Protocol = 0,
		.StringIndex = 0,
	},
	.Endpoint = {
		.DescriptorLength = 7,
		.DescriptorType = Endpoint,
		.EndpointAddress = {
			.Number = 1,
			.Direction = In,
		},
		.Attributes = {
			.Type = Interrupt,
		},
		.Packet = { 
			.MaxSize = 8,
		},
		.Interval = 0xff,
	},
};

static struct UsbStringDescriptor String0 = {
	.DescriptorLength = 4,
	.DescriptorType = String,
	.Data = {
		0x0409,
	},
};
static struct UsbStringDescriptor String1 = {
	.DescriptorLength = sizeof(L"USB 2.0 Root Hub") + 2,
	.DescriptorType = String,
	.Data = L"USB 2.0 Root Hub",
};

static struct HubDescriptor HubDescriptor = {
	.DescriptorLength = 0x9,
	.DescriptorType = Hub,
	.PortCount = 1,
	.Attributes = {
		.PowerSwitchingMode = Global,
		.Compound = false,
		.OverCurrentProtection = Global,
		.ThinkTime = 0,
		.Indicators = false,
	},
	.PowerGoodDelay = 0,
	.MaximumHubPower = 0,
	.Data = { 
		0x01,
		0xff,
	},
};

Result HcdProcessRootHubMessage(struct UsbDevice *device, struct UsbPipeAddress 
        pipe, void* buffer, u32 bufferLength,
		struct UsbDeviceRequest *request) {
	u32 replyLength;
	Result result;

	result = OK;
	device->Error = Processing;

	if (pipe.Type == Interrupt) {
		LOG("HCD.Hub: RootHub does not support IRQ pipes.\n");
		device->Error = Stall;
		return OK;
	}

	replyLength = 0;

	switch (request->Request) {
	case GetStatus:
		switch (request->Type) {
		case 0x80: // 
			*(u16*)buffer = 1;
			replyLength = 2;
			break;
		case 0x81: // interface
		case 0x82: // endpoint
			*(u16*)buffer = 0;
			replyLength = 2;
			break;
		case 0xa0: // class
			*(u32*)buffer = 0;
			replyLength = 4;
			break;
		case 0xa3:
			ReadBackReg(&Host->Port);
			
			*(u32*)buffer = 0;
			((struct HubPortFullStatus*)buffer)->Status.Connected = Host->Port.Connect;
			((struct HubPortFullStatus*)buffer)->Status.Enabled = Host->Port.Enable;
			((struct HubPortFullStatus*)buffer)->Status.Suspended = Host->Port.Suspend;
			((struct HubPortFullStatus*)buffer)->Status.OverCurrent = Host->Port.OverCurrent;
			((struct HubPortFullStatus*)buffer)->Status.Reset = Host->Port.Reset;
			((struct HubPortFullStatus*)buffer)->Status.Power = Host->Port.Power;
			if (Host->Port.Speed == High)
				((struct HubPortFullStatus*)buffer)->Status.HighSpeedAttatched = true;
			else if (Host->Port.Speed == Low)
				((struct HubPortFullStatus*)buffer)->Status.LowSpeedAttatched = true;
			((struct HubPortFullStatus*)buffer)->Status.TestMode = Host->Port.TestControl;
			((struct HubPortFullStatus*)buffer)->Change.ConnectedChanged = Host->Port.ConnectDetected;
			((struct HubPortFullStatus*)buffer)->Change.EnabledChanged = Host->Port.EnableChanged;
			((struct HubPortFullStatus*)buffer)->Change.OverCurrentChanged = Host->Port.OverCurrentChanged;
			((struct HubPortFullStatus*)buffer)->Change.ResetChanged = true;
			replyLength = 4;
			break;
		default:
			device->Error = Stall;
			break;
		};
		break;
	case ClearFeature:
		replyLength = 0;
		switch (request->Type) {
		case 0x2:
		case 0x20:
			break;
		case 0x23:
			switch ((enum HubPortFeature)request->Value) {
			case FeatureEnable:
				ReadBackReg(&Host->Port);
				Host->Port.Enable = true;
				WriteThroughRegMask(&Host->Port, 0x4);
				break;
			case FeatureSuspend:
				ClearReg(Power);
				WriteThroughReg(Power);
				MicroDelay(5000);
				ReadBackReg(&Host->Port);
				Host->Port.Resume = true;
				WriteThroughRegMask(&Host->Port, 0x40);
				MicroDelay(100000);
				Host->Port.Resume = false;
				Host->Port.Suspend = false;
				WriteThroughRegMask(&Host->Port, 0xc0);
				break;
			case FeaturePower:
				ReadBackReg(&Host->Port);
				Host->Port.Power = false;
				WriteThroughRegMask(&Host->Port, 0x1000);
				break;
			case FeatureConnectionChange:
				ReadBackReg(&Host->Port);
				Host->Port.ConnectDetected = true;
				WriteThroughRegMask(&Host->Port, 0x2);
				break;
			case FeatureEnableChange:
				ReadBackReg(&Host->Port);
				Host->Port.EnableChanged = true;
				WriteThroughRegMask(&Host->Port, 0x8);
				break;
			case FeatureOverCurrentChange:
				ReadBackReg(&Host->Port);
				Host->Port.OverCurrentChanged = true;
				WriteThroughRegMask(&Host->Port, 0x20);
				break;
			default:
				break;
			}
			break;
		default:
			result = ErrorArgument;
			break;
		}
		break;
	case SetFeature:
		replyLength = 0;
		switch (request->Type) {
		case 0x20:
			break;
		case 0x23:
			switch ((enum HubPortFeature)request->Value) {
			case FeatureReset:
				ReadBackReg(Power);
				Power->EnableSleepClockGating = false;
				Power->StopPClock = false;
				WriteThroughReg(Power);
				ClearReg(Power);
				WriteThroughReg(Power);

				ReadBackReg(&Host->Port);
				Host->Port.Suspend = false;
				Host->Port.Reset = true;
				Host->Port.Power = true;
				WriteThroughRegMask(&Host->Port, 0x1180);
				MicroDelay(60000);
				Host->Port.Reset = false;
				WriteThroughRegMask(&Host->Port, 0x1000);
				break;
			case FeaturePower:
				ReadBackReg(&Host->Port);
				Host->Port.Power = true;
				WriteThroughRegMask(&Host->Port, 0x1000);
				break;
			default:
				break;
			}
			break;
		default:
			result = ErrorArgument;
			break;
		}
		break;
	case SetAddress:
		replyLength = 0;
		RootHubDeviceNumber = request->Value;
		break;
	case GetDescriptor:
		switch (request->Type) {
		case 0x80:
			switch ((enum DescriptorType)((request->Value >> 8) & 0xff)) {
			case Device:
				MemoryCopy(buffer, &DeviceDescriptor, replyLength = Min(sizeof(DeviceDescriptor), bufferLength, u32));
				break;
			case Configuration:
				MemoryCopy(buffer, &ConfigurationDescriptor, replyLength = Min(sizeof(ConfigurationDescriptor), bufferLength, u32));
				break;
			case String:
				switch (request->Value & 0xff) {
				case 0x0:
					MemoryCopy(buffer, &String0, replyLength = Min(String0.DescriptorLength, bufferLength, u32));
					break;
				case 0x1:
					MemoryCopy(buffer, &String1, replyLength = Min(String1.DescriptorLength, bufferLength, u32));
					break;
				default:
					replyLength = 0;
					break;
				}
				break;
			default:
				result = ErrorArgument;
			}
			break;
		case 0xa0:
			MemoryCopy(buffer, &HubDescriptor, replyLength = Min(HubDescriptor.DescriptorLength, bufferLength, u32));
			break;
		default:
			result = ErrorArgument;
			break;
		}
		break;
	case GetConfiguration:
		*(u8*)buffer = 0x1;
		replyLength = 1;
		break;
	case SetConfiguration:
		replyLength = 0;
		break;
	default:
		result = ErrorArgument;
		break;
	}

	if (result == ErrorArgument)
		device->Error |= Stall;

	device->Error &= ~Processing;
	device->LastTransfer = replyLength;

	return OK;
}
/******************************************************************************
*	platform/platform.c
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	platform/platform.c contains code for generic system duties such as memory
*	management and logging, which can be optionally disabled. 
******************************************************************************/

#ifdef MEM_NO_RESERVE

static void* MemoryReserve(u32 length, void* physicalAddress) {
	return physicalAddress;
}

#endif

#ifdef MEM_INTERNAL_MANAGER_DEFAULT 

#define HEAP_END ((void*)0xFFFFFFFF)

struct HeapAllocation {
	u32 Length;
	void* Address;
	struct HeapAllocation *Next;
};

static u8 Heap[0x4000] __attribute__((aligned(8))) = {0}; // Support a maximum of 16KiB of allocations
static struct HeapAllocation Allocations[0x100] = {{0}}; // Support 256 allocations
static struct HeapAllocation *FirstAllocation = HEAP_END, *FirstFreeAllocation = NULL;
static u32 allocated = 0;

static void* MemoryAllocate(u32 size) {
	struct HeapAllocation *Current, *Next;
	if (FirstFreeAllocation == NULL) {
		LOG_DEBUG("Platform: First memory allocation, reserving 16KiB of heap, 256 entries.\n");
		MemoryReserve(sizeof(Heap), &Heap);
		MemoryReserve(sizeof(Allocations), &Allocations);

		FirstFreeAllocation = &Allocations[0];
	}

	size += (8 - (size & 7)) & 7; // Align to 8

	if (allocated + size > sizeof(Heap)) {
		LOG("Platform: Out of memory! We should've had more heap space in platform.c.\n");
		return NULL;
	}
	
	if (FirstFreeAllocation == HEAP_END) {
		LOG("Platform: Out of memory! We should've had more allocations in platform.c.\n");
		return NULL;
	}
	Current = FirstAllocation;

	while (Current != HEAP_END) {
		if (Current->Next != HEAP_END) {
			if ((u32)Current->Next->Address - (u32)Current->Address - Current->Length >= size) {
				FirstFreeAllocation->Address = (void*)((u8*)Current->Address + Current->Length);
				FirstFreeAllocation->Length = size;
				Next = FirstFreeAllocation;
				if (Next->Next == NULL)
					if ((u32)(FirstFreeAllocation + 1) < (u32)((u8*)Allocations + sizeof(Allocations)))
						FirstFreeAllocation = FirstFreeAllocation + 1;
					else
						FirstFreeAllocation = HEAP_END;
				else
					FirstFreeAllocation = Next->Next;
				Next->Next = Current->Next;
				Current->Next = Next;
				allocated += size;
				LOG_DEBUGF("Platform: malloc(%#x) = %#x. (%d/%d)\n", size, Next->Address, allocated, sizeof(Heap));
				return Next->Address;
			}
			else
				Current = Current->Next;
		} else {
			if ((u32)&Heap[sizeof(Heap)] - (u32)Current->Next - Current->Length >= size) {
				FirstFreeAllocation->Address = (void*)((u8*)Current->Address + Current->Length);
				FirstFreeAllocation->Length = size;
				Next = FirstFreeAllocation;
				if (Next->Next == NULL)
					if ((u32)(FirstFreeAllocation + 1) < (u32)((u8*)Allocations + sizeof(Allocations)))
						FirstFreeAllocation = FirstFreeAllocation + 1;
					else
						FirstFreeAllocation = HEAP_END;
				else
					FirstFreeAllocation = Next->Next;
				Next->Next = Current->Next;
				Current->Next = Next;
				allocated += size;
				LOG_DEBUGF("Platform: malloc(%#x) = %#x. (%d/%d)\n", size, Next->Address, allocated, sizeof(Heap));
				return Next->Address;
			}
			else {
				LOG("Platform: Out of memory! We should've had more heap space in platform.c.\n");
				LOG_DEBUGF("Platform: malloc(%#x) = %#x. (%d/%d)\n", size, NULL, allocated, sizeof(Heap));
				return NULL;
			}
		}
	}
	
	Next = FirstFreeAllocation->Next;
	FirstAllocation = FirstFreeAllocation;
	FirstAllocation->Next = HEAP_END;
	FirstAllocation->Length = size;
	FirstAllocation->Address = &Heap;
	if (Next == NULL)
		if ((u32)(FirstFreeAllocation + 1) < (u32)((u8*)Allocations + sizeof(Allocations)))
			FirstFreeAllocation = FirstFreeAllocation + 1;
		else
			FirstFreeAllocation = HEAP_END;
	else
		FirstFreeAllocation = Next;
	allocated += size;
	LOG_DEBUGF("Platform: malloc(%#x) = %#x. (%d/%d)\n", size, FirstAllocation->Address, allocated, sizeof(Heap));
	return FirstAllocation->Address;
}

static void MemoryDeallocate(void* address) {
	struct HeapAllocation *Current, **CurrentAddress;

	CurrentAddress = &FirstAllocation;
	Current = FirstAllocation;

	while (Current != HEAP_END) {
		if (Current->Address == address) {
			allocated -= Current->Length;
			*CurrentAddress = Current->Next;
			Current->Next = FirstFreeAllocation;
			FirstFreeAllocation = Current;
			LOG_DEBUGF("Platform: free(%#x) (%d/%d)\n", address, allocated, sizeof(Heap));
			return;
		}
		else {
			Current = Current->Next;
			CurrentAddress = &((*CurrentAddress)->Next);
		}
	}
	
	LOG_DEBUGF("Platform: free(%#x) (%d/%d)\n", address, allocated, sizeof(Heap));
	LOG("Platform: Deallocated memory that was never allocated. Ignored, but you should look into it.\n");
}

static void MemoryCopy(void* destination, void* source, u32 length) {
	u8 *d, *s;
	
	if (length == 0) return;

	d = (u8*)destination;
	s = (u8*)source;

	if ((u32)s < (u32)d)
		while (length-- > 0)
			*d++ = *s++;
	else {
		d += length;
		s += length;
		while (length-- > 0)
			*--d = *--s;
	}
}

#endif

#define FLOAT_TEXT "Floats unsupported."

#ifndef NO_LOG
/*
void LogPrintF(char* format, u32 formatLength, ...) {
	va_list args;
	char messageBuffer[160];
	u32 messageIndex, width = 0, precision = 1, characters;
	s32 svalue; u32 value;
	bool opened, flagged, widthed = false, precisioned = false, length = false;
	bool left = false, plus = false, space = false, hash = false, zero = false, nwidth = false, period = false, nprecision = false;
	bool longInt = false, shortInt = false;
	char character; char* string;
	u8 base;
	messageIndex = 0;
	opened = false;
	
	va_start(args, formatLength);

	for (u32 index = 0; index < formatLength && messageIndex < sizeof(messageBuffer) - 1; index++) {
		if (opened) {
			if (!flagged)
				switch (format[index]) {
				case '-':
					if (!left) left = true;
					else flagged = true;
					break;
				case '+':
					if (!plus) plus = true;
					else flagged = true;
					break;
				case ' ':
					if (!space) space = true;
					else flagged = true;
					break;
				case '#':
					if (!hash) hash = true;
					else flagged = true;
					break;
				case '0':
					if (!zero) zero = true;
					else flagged = true;
					break;
				default:
					flagged = true;
				}
			if (flagged && !widthed) {
				switch (format[index]) {
				case '0': case '1':
				case '2': case '3':
				case '4': case '5':
				case '6': case '7':
				case '8': case '9':
					nwidth = true;
					width *= 10;
					width += format[index] - '0';
					continue;
				case '*':
					if (!nwidth) {
						widthed = true;
						width = va_arg(args, u32);
						continue;
					}
					else
						widthed = true;
					break;
				default:
					widthed = true;
				}			
			}
			if (flagged && widthed && !precisioned) {
				if (!period) {
					if (format[index] == '.') {
						period = true;
						precision = 0;
						continue;
					} else
						precisioned = true;
				}
				else {
					switch (format[index]) {
					case '0': case '1':
					case '2': case '3':
					case '4': case '5':
					case '6': case '7':
					case '8': case '9':
						nprecision = true;
						precision *= 10;
						precision += format[index] - '0';
						continue;
					case '*':
						if (!nprecision) {
							precisioned = true;
							precision = va_arg(args, u32);
							continue;
						}
						else
							precisioned = true;
						break;
					default:
						precisioned = true;
					}
				}
			}
			if (flagged && widthed && precisioned && !length) {
				switch (format[index]) {
				case 'h':
					length = true;
					shortInt = true;
					continue;
				case 'l':
					length = true;
					longInt = true;
					continue;
				case 'L':
					length = true;
					continue;
				default:
					length = true;
				}
			}
			if (flagged && widthed && precisioned && length) {
				character = '%';
				base = 16;
				switch (format[index]) {
				case 'c':
					character = va_arg(args, int) & 0x7f;
				case '%':
					messageBuffer[messageIndex++] = character;
					break;
				case 'd':
				case 'i':
					if (shortInt) svalue = (s32)((s16)va_arg(args, s32) & 0xffff);
					else if (longInt) svalue = va_arg(args, s64);
					else svalue = va_arg(args, s32);

					characters = 1;
					if (svalue < 0) {
						svalue = -svalue;
						messageBuffer[messageIndex++] = '-';
					}
					else if (plus)
						messageBuffer[messageIndex++] = '-';
					else if (space)
						messageBuffer[messageIndex++] = ' ';
					else 
						characters = 0;

					for (u32 digits = 0; digits < precision || svalue > 0; digits++, characters++) {
						for (u32 j = 0; j < digits; j++)
							if (messageIndex - j < sizeof(messageBuffer) - 1)
								messageBuffer[messageIndex - j] = messageBuffer[messageIndex - j - 1];
						if (messageIndex - digits < sizeof(messageBuffer) - 1)
							messageBuffer[messageIndex++ -digits] = '0' + (svalue % 10);
						svalue /= 10;
					}		
					
					if (characters < width) {
						if (!left)
							for (u32 i = 0; i <= characters; i++) {
								if (messageIndex - characters + width - i < sizeof(messageBuffer) - 1)
									messageBuffer[messageIndex - characters + width - i] = 
										messageBuffer[messageIndex - i];
							}

						for (u32 digits = characters; characters < width; characters++) {
							if (messageIndex - (!left ? digits : 0) < sizeof(messageBuffer) - 1)
								messageBuffer[messageIndex - (!left ? digits : 0)] = zero ? '0' : ' '; 
						}
					}
					break;
				case 'e':
				case 'E':
				case 'f':
				case 'g':
				case 'G':
					for (u32 i = 0; (i < width || i < sizeof(FLOAT_TEXT)) && messageIndex < sizeof(messageBuffer) - 1; i++) {
						if (i < sizeof(FLOAT_TEXT))
							messageBuffer[messageIndex++] = FLOAT_TEXT[i];
						else 
							messageBuffer[messageIndex++] = zero ? '0' : ' ';
					}
					break;
				case 'o':
					base = 8;
				case 'u':
					if (format[index] == 'u') base = 10;
				case 'x':
				case 'X':
				case 'p':
					if (shortInt) value = va_arg(args, u32) & 0xffff;
					else if (longInt) value = va_arg(args, u64);
					else value = va_arg(args, u32);

					characters = 1;
					if (plus)
						messageBuffer[messageIndex++] = '-';
					else if (space)
						messageBuffer[messageIndex++] = ' ';
					else 
						characters = 0;

					if (hash) {
						if (format[index] == 'o') {
							if (messageIndex < sizeof(messageBuffer) - 1) 
								messageBuffer[messageIndex++] = '0';
							characters++;
						}
						else if (format[index] != 'u') {
							if (messageIndex < sizeof(messageBuffer) - 1) 
								messageBuffer[messageIndex++] = '0';
							characters++;
							if (messageIndex < sizeof(messageBuffer) - 1) 
								messageBuffer[messageIndex++] = format[index];
							characters++;
						}
					}
							

					for (u32 digits = 0; digits < precision || value > 0; digits++, characters++) {
						for (u32 j = 0; j < digits; j++)
							if (messageIndex - j < sizeof(messageBuffer) - 1)
								messageBuffer[messageIndex - j] = messageBuffer[messageIndex - j - 1];
						if (messageIndex - digits < sizeof(messageBuffer) - 1)
							messageBuffer[messageIndex++ -digits] =
							 (value % base) >= 10 ? format[index] - ('X' - 'A') + ((value % base) - 10) : '0' + (value % base);
						value /= base;
					}		

					if (characters < width) {
						if (!left)
							for (u32 i = 0; i <= characters; i++) {
								if (messageIndex - characters + width - i < sizeof(messageBuffer) - 1)
									messageBuffer[messageIndex - characters + width - i] = 
										messageBuffer[messageIndex - i];
							}

						for (u32 digits = characters; characters < width; characters++) {
							if (messageIndex - (!left ? digits : 0) < sizeof(messageBuffer) - 1)
								messageBuffer[messageIndex++ - (!left ? digits : 0)] = zero ? '0' : ' '; 
						}
					}
					break;
				case 's':
					string = va_arg(args, char*);
					for (u32 i = 0; messageIndex < sizeof(messageBuffer) - 1 && string[i] != '\0' && (!period || i < precision); i++) {
						messageBuffer[messageIndex++] = string[i];
					}
					break;
				case 'n':
					*va_arg(args, u32*) = messageIndex;
					break;
				}
				opened = false;
			}
		} else if (format[index] == '%') {
			opened = true;
			flagged = false;
			widthed = false;
			precisioned = false;
			length = false;
			width = 0; precision = 1;
			left = false; plus = false; space = false; hash = false; zero = false; nwidth = false; period = false; nprecision = false;
			longInt = false; shortInt = false;
		}
		else
			messageBuffer[messageIndex++] = format[index];
	}

	va_end(args);
	
	LogPrint(messageBuffer, messageIndex);
}
*/
#endif

static void PlatformLoad()
{
#ifdef MEM_INTERNAL_MANAGER_DEFAULT 
	FirstAllocation = HEAP_END;
	FirstFreeAllocation = NULL;
#endif
}
/******************************************************************************
*	platform/arm/armv6.c
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	platform/arm/armv6.c contains code for arm version 6 processors. Compiled
*	conditionally on LIB_ARM_V6=1.
******************************************************************************/

void Arm6Load()
{
	LOG_DEBUG("CSUD: ARMv6 driver version 0.1.\n");
}

#ifndef TYPE_DRIVER
u64 __aeabi_uidivmod(u32 value, u32 divisor) {
	u64 answer = 0;

	for (u32 i = 0; i < 32; i++) {
		if ((divisor << (31 - i)) >> (31 - i) == divisor) {
			if (value >= divisor << (31 - i)) {
				value -= divisor << (31 - i);
				answer |= 1 << (31 - i);
				if (value == 0) break;
			} 
		}
	}

	answer |= (u64)value << 32;
	return answer;
};

u32 __aeabi_uidiv(u32 value, u32 divisor) {
	return __aeabi_uidivmod(value, divisor);
};
#endif
/******************************************************************************
*	platform/arm/broadcom2835.c
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	platform/arm/broadcom2835.c contains code for the broadcom2835 chip, used 
*	in the Raspberry Pi. Compiled conditionally on LIB_BCM2835=1.
******************************************************************************/

void Bcm2835Load()
{
	LOG_DEBUG("CSUD: Broadcom2835 driver version 0.1.\n");
}

#ifndef TYPE_DRIVER

static void MicroDelay(u32 delay) {
	volatile u64* timeStamp = (u64*)0x20003004;
	u64 stop = *timeStamp + delay;

	while (*timeStamp < stop) 
		__asm__("nop");
}

static Result PowerOnUsb() {
	volatile u32* mailbox;
	u32 result;

	mailbox = (u32*)0x2000B880;
	while (mailbox[6] & 0x80000000);
	mailbox[8] = 0x80;
	do {
		while (mailbox[6] & 0x40000000);		
	} while (((result = mailbox[0]) & 0xf) != 0);
	return result == 0x80 ? OK : ErrorDevice;
}

#endif
/******************************************************************************
*	usbd/usbd.c
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	usbd.c contains code relating to the generic USB driver. USB
*	is designed such that this driver's interface would be virtually the same
*	across all systems, and in fact its implementation varies little either.
******************************************************************************/

/**
	\brief Scans the bus for hubs.

	Enumerates the devices on the bus, performing necessary initialisation and 
	building a device tree for later reference.
*/
static Result UsbAttachRootHub();

struct UsbDevice *UsbGetRootHub() { return Devices[0]; }

static void UsbLoad()
{	
	LOG("CSUD: USB driver version 0.1\n"); 
	for (u32 i = 0; i < MaximumDevices; i++)
		Devices[i] = NULL;
	for (u32 i = 0; i < InterfaceClassAttachCount; i++)
		InterfaceClassAttach[i] = NULL;
}

Result UsbInitialise() {
	Result result;

	ConfigurationLoad();
	result = OK;

	if (sizeof(struct UsbDeviceRequest) != 0x8) {
		LOGF("USBD: Incorrectly compiled driver. UsbDeviceRequest: %#x (0x8).\n", 
			sizeof(struct UsbDeviceRequest));
		return ErrorCompiler; // Correct packing settings are required.
	}

	if ((result = HcdInitialise()) != OK) {
		LOG("USBD: Abort, HCD failed to initialise.\n");
		goto errorReturn;
	}
	if ((result = HcdStart()) != OK) {
		LOG("USBD: Abort, HCD failed to start.\n");
		goto errorDeinitialise;
	}
	if ((result = UsbAttachRootHub()) != OK) {
		LOG("USBD: Failed to enumerate devices.\n");
		goto errorStop;
	}

	return result;
errorStop:
	HcdStop();
errorDeinitialise:
	HcdDeinitialise();
errorReturn:
	return result;
}

static Result UsbControlMessage(struct UsbDevice *device, struct UsbPipeAddress 
    pipe, void* buffer, u32 bufferLength,
	struct UsbDeviceRequest *request, u32 timeout) {
	Result result;

	if (((u32)buffer & 0x3) != 0)
		LOG_DEBUG("USBD: Warning message buffer not word aligned.\n");
	result = HcdSumbitControlMessage(device, pipe, buffer, bufferLength, request);

	if (result != OK) {
		LOG_DEBUGF("USBD: Failed to send message to %s: %d.\n", UsbGetDescription(device), result);
		return result;
	}

	while (timeout-- > 0 && (device->Error & Processing))
		MicroDelay(1000);

	if ((device->Error & Processing)) {
		LOG_DEBUGF("USBD: Message to %s timeout reached.\n", UsbGetDescription(device));
		return ErrorTimeout;
	}

	if (device->Error & ~Processing) {
		if (device->Parent != NULL && device->Parent->DeviceCheckConnection != NULL) {
			// Check we're still connected!
			LOG_DEBUGF("USBD: Verifying %s is still connected.\n", UsbGetDescription(device));
			if ((result = device->Parent->DeviceCheckConnection(device->Parent, device)) != OK) {
				return ErrorDisconnected;
			}		
			LOG_DEBUGF("USBD: Yes, %s is still connected.\n", UsbGetDescription(device));
		}
		result = ErrorDevice;
	}

	return result;
}

static Result UsbGetDescriptor(struct UsbDevice *device, enum DescriptorType 
    type, u8 index, u16 langId, void* buffer, u32 length, u32 minimumLength, u8 
recipient) {
	Result result;

	if ((result = UsbControlMessage(
		device, 
		(struct UsbPipeAddress) { 
			.Type = Control, 
			.Speed = device->Speed, 
			.EndPoint = 0 , 
			.Device = device->Number, 
			.Direction = In,
			.MaxSize = SizeFromNumber(device->Descriptor.MaxPacketSize0),
		},
		buffer,
		length,
		&(struct UsbDeviceRequest) {
			.Request = GetDescriptor,
			.Type = 0x80 | recipient,
			.Value = (u16)type << 8 | index,
			.Index = langId,
			.Length = length,
		},
		ControlMessageTimeout)) != OK) {
			LOGF("USBD: Failed to get descriptor %#x:%#x for device %s. Result %#x.\n", type, index, UsbGetDescription(device), result);
		return result;
	}

	if (device->LastTransfer < minimumLength) {
		LOGF("USBD: Unexpectedly short descriptor (%d/%d) %#x:%#x for device %s. Result %#x.\n", device->LastTransfer, minimumLength, type, index, UsbGetDescription(device), result);
		return ErrorDevice;
	}

	return OK;
}
	
static Result UsbGetString(struct UsbDevice *device, u8 stringIndex, u16 
    langId, void* buffer, u32 length) {
	Result result;
	
	// Apparently this tends to fail a lot.
	for (u32 i = 0; i < 3; i++) {
		result = UsbGetDescriptor(device, String, stringIndex, langId, buffer, length, length, 0);

		if (result == OK)
			break;
	}

	return result;
}

static Result UsbReadStringLang(struct UsbDevice *device, u8 stringIndex, u16 
    langId, void* buffer, u32 length) {
	Result result;
	
	result = UsbGetString(device, stringIndex, langId, buffer, Min(2, length, u32));

	if (result == OK && device->LastTransfer != length)
		result = UsbGetString(device, stringIndex, langId, buffer, Min(((u8*)buffer)[0], length, u32));

	return result;
}

static Result UsbReadString(struct UsbDevice *device, u8 stringIndex, char* 
    buffer, u32 length) {
	Result result; 
	u32 i; u8 descriptorLength;

	if (buffer == NULL || stringIndex == 0)
		return ErrorArgument;
	u16 langIds[2];
	struct UsbStringDescriptor *descriptor;

	result = UsbReadStringLang(device, 0, 0, &langIds, 4);

	if (result != OK) {
		LOGF("USBD: Error getting language list from %s.\n", UsbGetDescription(device));
		return result;
	}
	else if (device->LastTransfer < 4) {
		LOGF("USBD: Unexpectedly short language list from %s.\n", UsbGetDescription(device));
		return ErrorDevice;
	}

	descriptor = (struct UsbStringDescriptor*)buffer;
	if (descriptor == NULL)
		return ErrorMemory;
	if ((result = UsbReadStringLang(device, stringIndex, langIds[1], descriptor, length)) != OK)
		return result;
 
	descriptorLength = descriptor->DescriptorLength;
	for (i = 0; i < length - 1 && i < (descriptorLength - 2) >> 1; i++) {
		if (descriptor->Data[i] > 0xff)
			buffer[i] = '?';
		else {
			buffer[i] = descriptor->Data[i];
		}

	};

	if (i < length)
		buffer[i++] = '\0';

	return result;
}

static Result UsbReadDeviceDescriptor(struct UsbDevice *device) {
	Result result;
	if (device->Speed == Low) {
		device->Descriptor.MaxPacketSize0 = 8;
		if ((result = UsbGetDescriptor(device, Device, 0, 0, (void*)&device->Descriptor, sizeof(device->Descriptor), 8, 0)) != OK)
			return result;
		if (device->LastTransfer == sizeof(struct UsbDeviceDescriptor))
			return result;
		return UsbGetDescriptor(device, Device, 0, 0, (void*)&device->Descriptor, sizeof(device->Descriptor), sizeof(device->Descriptor), 0);
	} else if (device->Speed == Full) {
		device->Descriptor.MaxPacketSize0 = 64;
		if ((result = UsbGetDescriptor(device, Device, 0, 0, (void*)&device->Descriptor, sizeof(device->Descriptor), 8, 0)) != OK)
			return result;
		if (device->LastTransfer == sizeof(struct UsbDeviceDescriptor))
			return result;
		return UsbGetDescriptor(device, Device, 0, 0, (void*)&device->Descriptor, sizeof(device->Descriptor), sizeof(device->Descriptor), 0);
	} else {
		device->Descriptor.MaxPacketSize0 = 64;
		return UsbGetDescriptor(device, Device, 0, 0, (void*)&device->Descriptor, sizeof(device->Descriptor), sizeof(device->Descriptor), 0);
	}
}

static Result UsbSetAddress(struct UsbDevice *device, u8 address) {
	Result result;
	
	if (device->Status != Default) {
		LOGF("USBD: Illegal attempt to configure device %s in state %#x.\n", UsbGetDescription(device), device->Status);
		return ErrorDevice;
	}

	if ((result = UsbControlMessage(
		device, 
		(struct UsbPipeAddress) { 
			.Type = Control, 
			.Speed = device->Speed, 
			.EndPoint = 0, 
			.Device = 0, 
			.Direction = Out,
			.MaxSize = SizeFromNumber(device->Descriptor.MaxPacketSize0),
		},
		NULL,
		0,
		&(struct UsbDeviceRequest) {
			.Request = SetAddress,
			.Type = 0,
			.Value = address,
		},
		ControlMessageTimeout)) != OK) 
		return result;

	MicroDelay(10000); // Allows the address to propagate.
	device->Number = address;
	device->Status = Addressed;
	return OK;
}

static Result UsbSetConfiguration(struct UsbDevice *device, u8 configuration) {
	Result result;
	
	if (device->Status != Addressed) {
		LOGF("USBD: Illegal attempt to configure device %s in state %#x.\n", UsbGetDescription(device), device->Status);
		return ErrorDevice;
	}

	if ((result = UsbControlMessage(
		device, 
		(struct UsbPipeAddress) { 
			.Type = Control, 
			.Speed = device->Speed, 
			.EndPoint = 0, 
			.Device = device->Number, 
			.Direction = Out,
			.MaxSize = SizeFromNumber(device->Descriptor.MaxPacketSize0),
		},
		NULL,
		0,
		&(struct UsbDeviceRequest) {
			.Request = SetConfiguration,
			.Type = 0,
			.Value = configuration,
		},
		ControlMessageTimeout)) != OK)
		return result;

	device->ConfigurationIndex = configuration;
	device->Status = Configured;
	return OK;	
}

static Result UsbConfigure(struct UsbDevice *device, u8 configuration) {
	Result result;
	void* fullDescriptor;
	struct UsbDescriptorHeader *header;
	struct UsbInterfaceDescriptor *interface;
	struct UsbEndpointDescriptor *endpoint;
	u32 lastInterface, lastEndpoint;
	bool isAlternate;
		
	if (device->Status != Addressed) {
		LOGF("USBD: Illegal attempt to configure device %s in state %#x.\n", UsbGetDescription(device), device->Status);
		return ErrorDevice;
	}
		
	if ((result = UsbGetDescriptor(device, Configuration, configuration, 0, (void*)&device->Configuration, sizeof(device->Configuration), sizeof(device->Configuration), 0)) != OK) {
		LOGF("USBD: Failed to retrieve configuration descriptor %#x for device %s.\n", configuration, UsbGetDescription(device));
		return result;
	} 

	if ((fullDescriptor = MemoryAllocate(device->Configuration.TotalLength)) == NULL) {
		LOG("USBD: Failed to allocate space for descriptor.\n");
		return ErrorMemory;
	}
	if ((result = UsbGetDescriptor(device, Configuration, configuration, 0, fullDescriptor, device->Configuration.TotalLength, device->Configuration.TotalLength, 0)) != OK) {
		LOGF("USBD: Failed to retrieve full configuration descriptor %#x for device %s.\n", configuration, UsbGetDescription(device));
		goto deallocate;
	}

	device->ConfigurationIndex = configuration;
	configuration = device->Configuration.ConfigurationValue;

	header = fullDescriptor;
	lastInterface = MaxInterfacesPerDevice;
	lastEndpoint = MaxEndpointsPerDevice;
	isAlternate = false;

	for (header = (struct UsbDescriptorHeader*)((u8*)header + header->DescriptorLength);
		(u32)header - (u32)fullDescriptor < device->Configuration.TotalLength;
		header = (struct UsbDescriptorHeader*)((u8*)header + header->DescriptorLength)) {
		switch (header->DescriptorType) {
		case Interface:
			interface = (struct UsbInterfaceDescriptor*)header;
			if (lastInterface != interface->Number) {
				MemoryCopy((void*)&device->Interfaces[lastInterface = interface->Number], (void*)interface, sizeof(struct UsbInterfaceDescriptor));
				lastEndpoint = 0;
				isAlternate = false;
			}
			else isAlternate = true;
			break;
		case Endpoint:
			if (isAlternate) break;
			if (lastInterface == MaxInterfacesPerDevice || lastEndpoint >= device->Interfaces[lastInterface].EndpointCount) {
				LOG_DEBUGF("USBD: Unexpected endpoint descriptor in %s.Interface%d.\n", UsbGetDescription(device), lastInterface + 1);
				break;
			}
			endpoint = (struct UsbEndpointDescriptor*)header;
			MemoryCopy((void*)&device->Endpoints[lastInterface][lastEndpoint++], (void*)endpoint, sizeof(struct UsbEndpointDescriptor));
			break;
		default:
			if (header->DescriptorLength == 0)
				goto headerLoopBreak;

			break;
		}
		
		LOG_DEBUGF("USBD: Descriptor %d length %d, interface %d.\n", header->DescriptorType, header->DescriptorLength, lastInterface);
	}
headerLoopBreak:

	if ((result = UsbSetConfiguration(device, configuration)) != OK) {		
		goto deallocate;
	}
	LOG_DEBUGF("USBD: %s configuration %d. Class %d, subclass %d.\n", UsbGetDescription(device), configuration, device->Interfaces[0].Class, device->Interfaces[0].SubClass);

	device->FullConfiguration = fullDescriptor;
	 
	return OK;
deallocate:
	MemoryDeallocate(fullDescriptor);
	return result;
}

const char* UsbGetDescription(struct UsbDevice *device) {
	if (device->Status == Attached)
		return "New Device (Not Ready)\0";
	else if (device->Status == Powered)
		return "Unknown Device (Not Ready)\0";
	else if (device == Devices[0])
		return "USB Root Hub\0";

	switch (device->Descriptor.Class) {
	case DeviceClassHub:
		if (device->Descriptor.UsbVersion == 0x210)
			return "USB 2.1 Hub\0";
		else if (device->Descriptor.UsbVersion == 0x200)
			return "USB 2.0 Hub\0";
		else if (device->Descriptor.UsbVersion == 0x110)
			return "USB 1.1 Hub\0";
		else if (device->Descriptor.UsbVersion == 0x100)
			return "USB 1.0 Hub\0";
		else
			return "USB Hub\0";
	case DeviceClassVendorSpecific:
		if (device->Descriptor.VendorId == 0x424 &&
			device->Descriptor.ProductId == 0xec00)
			return "SMSC LAN9512\0";
	case DeviceClassInInterface:
		if (device->Status == Configured) {
			switch (device->Interfaces[0].Class) {
			case InterfaceClassAudio:
				return "USB Audio Device\0";
			case InterfaceClassCommunications:
				return "USB CDC Device\0";
			case InterfaceClassHid:
				switch (device->Interfaces[0].Protocol) {
				case 1:
					return "USB Keyboard\0";
				case 2:
					return "USB Mouse\0";					
				default:
					return "USB HID\0";
				}
			case InterfaceClassPhysical:
				return "USB Physical Device\0";
			case InterfaceClassImage:
				return "USB Imaging Device\0";
			case InterfaceClassPrinter:
				return "USB Printer\0";
			case InterfaceClassMassStorage:
				return "USB Mass Storage Device\0";
			case InterfaceClassHub:
				if (device->Descriptor.UsbVersion == 0x210)
					return "USB 2.1 Hub\0";
				else if (device->Descriptor.UsbVersion == 0x200)
					return "USB 2.0 Hub\0";
				else if (device->Descriptor.UsbVersion == 0x110)
					return "USB 1.1 Hub\0";
				else if (device->Descriptor.UsbVersion == 0x100)
					return "USB 1.0 Hub\0";
				else
					return "USB Hub\0";
			case InterfaceClassCdcData:
				return "USB CDC-Data Device\0";
			case InterfaceClassSmartCard:
				return "USB Smart Card\0";
			case InterfaceClassContentSecurity:
				return "USB Content Secuity Device\0";
			case InterfaceClassVideo:
				return "USB Video Device\0";
			case InterfaceClassPersonalHealthcare:
				return "USB Healthcare Device\0";
			case InterfaceClassAudioVideo:
				return "USB AV Device\0";
			case InterfaceClassDiagnosticDevice:
				return "USB Diagnostic Device\0";
			case InterfaceClassWirelessController:
				return "USB Wireless Controller\0";
			case InterfaceClassMiscellaneous:
				return "USB Miscellaneous Device\0";
			case InterfaceClassVendorSpecific:
				return "Vendor Specific\0";
			default:
				return "Generic Device\0";
			}
		} else if (device->Descriptor.Class == DeviceClassVendorSpecific) 
			return "Vendor Specific\0";
		else	
			return "Unconfigured Device\0";		
	default:
		return "Generic Device\0";
	}
}

static Result UsbAttachDevice(struct UsbDevice *device) {
	Result result;
	u8 address;
	char* buffer;

	// Store the address until it is actually assigned.
	address = device->Number;
	device->Number = 0;
	LOG_DEBUGF("USBD: Scanning %d. %s.\n", address, SpeedToChar(device->Speed));

	if ((result = UsbReadDeviceDescriptor(device)) != OK) {
		LOGF("USBD: Failed to read device descriptor for %d.\n", address);
		device->Number = address;
		return result;
	}
	device->Status = Default;

	if (device->Parent != NULL && device->Parent->DeviceChildReset != NULL) {
		// Reset the port for what will be the second time.
		if ((result = device->Parent->DeviceChildReset(device->Parent, device)) != OK) {
			LOGF("USBD: Failed to reset port again for new device %s.\n", UsbGetDescription(device));
			device->Number = address;
			return result;
		}
	}

	if ((result = UsbSetAddress(device, address)) != OK) {
		LOGF("USBD: Failed to assign address to %#x.\n", address);
		device->Number = address;
		return result;
	}
	device->Number = address;

	if ((result = UsbReadDeviceDescriptor(device)) != OK) {
		LOGF("USBD: Failed to reread device descriptor for %#x.\n", address);
		return result;
	}

	LOG_DEBUGF("USBD: Attach Device %s. Address:%d Class:%d Subclass:%d USB:%x.%x. %d configurations, %d interfaces.\n",
		UsbGetDescription(device), address, device->Descriptor.Class, device->Descriptor.SubClass, device->Descriptor.UsbVersion >> 8, 
		(device->Descriptor.UsbVersion >> 4) & 0xf, device->Descriptor.ConfigurationCount, device->Configuration.InterfaceCount);
		
	LOGF("USBD: Device Attached: %s.\n", UsbGetDescription(device));	
	buffer = NULL;
	
	if (device->Descriptor.Product != 0) {
		if (buffer == NULL) buffer = MemoryAllocate(0x100);		
		if (buffer != NULL) {
			result = UsbReadString(device, device->Descriptor.Product, buffer, 0x100);
			if (result == OK) LOGF("USBD:  -Product:       %s.\n", buffer);	
		}
	}	
	if (device->Descriptor.Manufacturer != 0) {
		if (buffer == NULL) buffer = MemoryAllocate(0x100);		
		if (buffer != NULL) {
			result = UsbReadString(device, device->Descriptor.Manufacturer, buffer, 0x100);
			if (result == OK) LOGF("USBD:  -Manufacturer:  %s.\n", buffer);	
		}
	}
	if (device->Descriptor.SerialNumber != 0) {
		if (buffer == NULL) buffer = MemoryAllocate(0x100);		
		if (buffer != NULL) {
			result = UsbReadString(device, device->Descriptor.SerialNumber, buffer, 0x100);
			if (result == OK) LOGF("USBD:  -SerialNumber:  %s.\n", buffer);	
		}
	}
	
	if (buffer != NULL) { MemoryDeallocate(buffer); buffer = NULL; }
	
	LOG_DEBUGF("USBD:  -VID:PID:       %x:%x v%d.%x\n", device->Descriptor.VendorId, device->Descriptor.ProductId, device->Descriptor.Version >> 8, device->Descriptor.Version & 0xff);
	
	// We only support devices with 1 configuration for now.
	if ((result = UsbConfigure(device, 0)) != OK) {
		LOGF("USBD: Failed to configure device %#x.\n", address);
		return OK;
	}

	if (device->Configuration.StringIndex != 0) {
		if (buffer == NULL) buffer = MemoryAllocate(0x100);			
		if (buffer != NULL) {
			result = UsbReadString(device, device->Configuration.StringIndex, buffer, 0x100);
			if (result == OK) LOGF("USBD:  -Configuration: %s.\n", buffer);	
		}
	}
	
	if (buffer != NULL) { MemoryDeallocate(buffer); buffer = NULL; }
					
	if (device->Interfaces[0].Class < InterfaceClassAttachCount &&
		InterfaceClassAttach[device->Interfaces[0].Class] != NULL) {		
		if ((result = InterfaceClassAttach[device->Interfaces[0].Class](device, 0)) != OK) {
			LOGF("USBD: Could not start the driver for %s.\n", UsbGetDescription(device));
		}
	}

	return OK;
}

static Result UsbAllocateDevice(struct UsbDevice **device) {
	if ((*device = MemoryAllocate(sizeof(struct UsbDevice))) == NULL)
		return ErrorMemory;

	for (u32 number = 0; number < MaximumDevices; number++) {
		if (Devices[number] == NULL) {
			Devices[number] = *device;
			(*device)->Number = number + 1;
			break;
		}
	}

	LOG_DEBUGF("USBD: Allocating new device, address %#x.\n", (*device)->Number);

	(*device)->Status = Attached;
	(*device)->Error = None;
	(*device)->PortNumber = 0;
	(*device)->Parent = NULL;
	(*device)->DriverData = NULL;
	(*device)->FullConfiguration = NULL;
	(*device)->ConfigurationIndex = 0xff;
	(*device)->DeviceDeallocate = NULL;
	(*device)->DeviceDetached = NULL;
	(*device)->DeviceCheckConnection = NULL;
	(*device)->DeviceCheckForChange = NULL;	
	(*device)->DeviceChildDetached = NULL;	
	(*device)->DeviceChildReset = NULL;
	return OK;
}

static void UsbDeallocateDevice(struct UsbDevice *device) {
	LOG_DEBUGF("USBD: Deallocating device %d: %s.\n", device->Number, UsbGetDescription(device));
	
	if (device->DeviceDetached != NULL)
		device->DeviceDetached(device);
	if (device->DeviceDeallocate != NULL)
		device->DeviceDeallocate(device);
	
	if (device->Parent != NULL && device->Parent->DeviceChildDetached != NULL)
		device->Parent->DeviceChildDetached(device->Parent, device);

	if (device->Status == Addressed || device->Status == Configured)
		if (device->Number > 0 && device->Number <= MaximumDevices && Devices[device->Number - 1] == device)
			Devices[device->Number - 1] = NULL;
	
	if (device->FullConfiguration != NULL)
		MemoryDeallocate((void *)device->FullConfiguration);

	MemoryDeallocate(device);
}

static Result UsbAttachRootHub() {
	Result result;
	struct UsbDevice *rootHub;
	LOG_DEBUG("USBD: Scanning for devices.\n");
	if (Devices[0] != NULL)
		UsbDeallocateDevice(Devices[0]);
	if ((result = UsbAllocateDevice(&rootHub)) != OK)
		return result;
		
	Devices[0]->Status = Powered;

	return UsbAttachDevice(Devices[0]);
}

void UsbCheckForChange(void) {
	if (Devices[0] != NULL &&
		Devices[0]->DeviceCheckForChange != NULL)
		Devices[0]->DeviceCheckForChange(Devices[0]);
}

