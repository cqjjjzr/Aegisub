cd %1

icacls data /grant:r %USERNAME%:F /T
rd /s /q data