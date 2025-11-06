#include <iostream>
#include <libusb-1.0/libusb.h>

void print_device(libusb_device* dev);

int main()
{
    libusb_context* ctx = nullptr;
    libusb_device** devs = nullptr;

    int r = libusb_init(&ctx);
    if (r < 0) {
        std::cerr << "Ошибка инициализации libusb: " << std::endl;
        return 1;
    }

    ssize_t cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        std::cerr << "Ошибка получения списка устройств: " << std::endl;
        libusb_exit(ctx);
        return 1;
    }

    std::cout << "Найдено устройств: " << cnt << "\n\n";

    for (ssize_t i = 0; i < cnt; ++i) {
        print_device(devs[i]);
    }

    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);
    return 0;
}

void print_device(libusb_device* dev)
{
    libusb_device_descriptor desc{};
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
        std::cerr << "Ошибка получения дескриптора устройства: " << std::endl;
        return;
    }

    std::cout << "---------------------------------------------------\n";
    std::cout << "Класс устройства:  0x" << std::hex << static_cast<int>(desc.bDeviceClass) << std::dec << "\n";

    libusb_device_handle* handle = nullptr;
    r = libusb_open(dev, &handle);

    if (r == 0 && handle != nullptr) {

        if (desc.iManufacturer != 0) {
            unsigned char buffer[256];
            int length = libusb_get_string_descriptor_ascii(handle,desc.iManufacturer,buffer,sizeof(buffer));
            if (length > 0) {
                std::cout << "производитель: " << buffer << std::dec << "\n";
            } else {
                std::cout << "ID производителя:  0x" << std::hex << desc.idVendor << std::dec << "\n";
            }
        } else {
            std::cout << "ID производителя недоступен" << "\n";
        }

        if (desc.idProduct != 0) {
            unsigned char buffer[256];
            int length = libusb_get_string_descriptor_ascii(handle,desc.idProduct,buffer,sizeof(buffer));
            if (length > 0) {
                std::cout << "изделие: " << buffer << "\n";
            } else {
                std::cout << "ID изделия:        0x" << std::hex << desc.idProduct << std::dec << "\n";
            }
        } else {
            std::cout << "ID изделия недоступен" << "\n";
        }


        if (desc.iSerialNumber) {
            unsigned char serial[256];
            int ret = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, serial, sizeof(serial));

            if (ret > 0) {
                std::cout << "Серийный номер:    " << serial << "\n";
            } else {
                std::cout << "Серийный номер:    <недоступен>\n";
            }
        } else {
            std::cout << "Серийный номер:    <отсутствует>\n";
        }

        libusb_close(handle);
    } else {
        std::cout << "ID производителя:  0x" << std::hex << desc.idVendor << std::dec << "\n";
        std::cout << "ID изделия:        0x" << std::hex << desc.idProduct << std::dec << "\n";
        std::cout << "Не удалось открыть устройство для получения серийного номера\n";
    }

    std::cout.flush();
}
