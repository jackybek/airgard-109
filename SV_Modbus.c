#include <stdio.h>
#include <modbus/modbus.h>

modbus_t *initialiseModbusConnection(char* ipaddress);
int readModbusData(modbus_t *mb);
void closeModbusConnection(modbus_t *mb);


modbus_t *initialiseModbusConnection(char* ipaddress) {
  modbus_t *mb=NULL;

  mb = modbus_new_tcp(ipaddress, 502);
  modbus_connect(mb);

  return mb;
}

int readModbusData(modbus_t *mb)
{
  uint16_t tab_reg[32];

  /* Read 5 registers from the address 0 */
  if (mb)
  {
  	modbus_read_registers(mb, 0, 5, tab_reg);
	return 0;
  }
  else return -1;
}

void closeModbusConnection(modbus_t *mb)
{
  modbus_close(mb);
  modbus_free(mb);
}
