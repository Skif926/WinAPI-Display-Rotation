/*
 * This file is part of WinAPI Display Rotation.
 *
 * WinAPI Display Rotation is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WinAPI Display Rotation is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include <windows.h>
#include <utility>
#include <stdexcept>
#include <exception>
#include <string>

class DisplayRotator {
public:
    enum Orientation {
        DEFAULT = DMDO_DEFAULT,
        ROTATE_90 = DMDO_90,
        ROTATE_180 = DMDO_180,
        ROTATE_270 = DMDO_270
    };

    DisplayRotator() {
        deviceName_[0] = '\0';

        int monitorIndex = findMonitor();
        if (monitorIndex == -1) {
            // Если не нашли второй монитор, будем использовать основной (NULL)
            deviceName_[0] = '\0';
        } else {
            // Получаем имя устройства второго монитора
            DISPLAY_DEVICE dd = {};
            dd.cb = sizeof(dd);
            if (!EnumDisplayDevices(NULL, monitorIndex, &dd, 0)) {
                throw std::runtime_error("Failed to get display device name");
            }
            strcpy_s(deviceName_, dd.DeviceName);
        }

        dm_ = {};
        dm_.dmSize = sizeof(dm_);

        // Получаем настройки выбранного монитора
        if (!EnumDisplaySettings(deviceName_[0] ? deviceName_ : NULL, ENUM_CURRENT_SETTINGS, &dm_)) {
            throw std::runtime_error("Failed to get display settings");
        }
    }

    bool setOrientation(Orientation newOrientation) {
        if (dm_.dmDisplayOrientation == DMDO_90 || dm_.dmDisplayOrientation == DMDO_270) {
            std::swap(dm_.dmPelsWidth, dm_.dmPelsHeight);
        }

        dm_.dmDisplayOrientation = newOrientation;

        if (newOrientation == DMDO_90 || newOrientation == DMDO_270) {
            std::swap(dm_.dmPelsWidth, dm_.dmPelsHeight);
        }

        dm_.dmFields = DM_DISPLAYORIENTATION | DM_PELSWIDTH | DM_PELSHEIGHT;

        LONG result = ChangeDisplaySettingsEx(deviceName_[0] ? deviceName_ : NULL, &dm_, NULL, CDS_UPDATEREGISTRY, NULL);
        return (result == DISP_CHANGE_SUCCESSFUL);
    }

private:
    DEVMODE dm_;
    char deviceName_[32];

    // Возвращает индекс второго монитора, если есть, иначе -1
    int findMonitor() {
        int count = 0;
        DISPLAY_DEVICE dd = {};
        dd.cb = sizeof(dd);
        for (DWORD i = 0; EnumDisplayDevices(NULL, i, &dd, 0); ++i) {
            // Пропускаем неактивные и драйверы зеркалирования
            if ((dd.StateFlags & DISPLAY_DEVICE_ACTIVE) && !(dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)) {
                ++count;
                if (count == 2) {
                    return i;  // Второй монитор найден
                }
            }
        }
        return -1;  // Второго монитора нет
    }
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    try {
        DisplayRotator rotator;
        bool success = rotator.setOrientation(DisplayRotator::DEFAULT);
        //bool success = rotator.setOrientation(DisplayRotator::ROTATE_90);
        //bool success = rotator.setOrientation(DisplayRotator::ROTATE_180);
        //bool success = rotator.setOrientation(DisplayRotator::ROTATE_270);
        return success ? 0 : 1;
    }
    catch (const std::exception&) {
        return 1;
    }
}

