/*
 * Copyright 2011 Frank Maker <frank.maker@gmail.com>
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the license, or (at your option) any later
 * version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; WITHOUT EVEN THE IMPLIED WARRANTY OF
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <linux/module.h>
#include <linux/spi/spi.h>

#define SPI_BUS1     1

static struct spi_device *mcp3901_device;

static  __initdata struct spi_board_info mcp3901_board_info = {
	.modalias	= "mcp3901",
	.platform_data  = NULL,
	.mode           = SPI_MODE_0,
	.irq            = NULL,
	.max_speed_hz   = 1000,
	.bus_num        = SPI_BUS1,
	.chip_select    = 0,

};


struct mcp3901_state {
	struct spi_device	*dev;
	struct mutex            lock;
};

/* Bind driver to board_info using modalias "mcp3901" */
static int __devinit mcp3901_probe(struct spi_device *spi)
{
	struct mcp3901_state             *state;
	struct mcp3901_platform_data     *pdata;

	pdata = spi->dev.platform_data;
	if(!pdata)
		return -ENODEV;

	state = kzalloc(sizeof(struct mcp3901_state), GFP_KERNEL);
	if(!state)
		return -ENOMEM;

	spi_set_drvdata(spi, state);

	return 0;
}

static int __devexit mcp3901_remove(struct spi_device *spi)
{
	struct mcp3901_state *st = spi_get_drvdata(spi);
	kfree(st);
	return 0;
}

static struct spi_driver mcp3901_driver = {
	.driver = {
		.name          = "mcp3901",
		.owner         = THIS_MODULE,
	},
	
	.probe           = mcp3901_probe,
	.remove          = __devexit_p(mcp3901_remove),

};

static int __init mcp3901_hotplug(void)
{
	int ret;
	struct spi_master     *master;

	master = spi_busnum_to_master(SPI_BUS1);
	if(master == NULL)
		return -ENODEV;

	mcp3901_device = spi_new_device(master, &mcp3901_board_info);
	if(mcp3901_device == NULL)
		return -ENODEV;

	printk("Hotplugged MCP3901.");
	return 0;
}

static int __init mcp3901_init(void)
{
	int ret;

	ret = spi_register_driver(&mcp3901_driver);
	if(ret)
		goto error;

	ret = mcp3901_hotplug();
	if(ret)
		goto error;

	return 0;

error:
	return ret;

}
module_init(mcp3901_init);

static void __exit mcp3901_exit(void)
{
	spi_unregister_device(mcp3901_device);
	spi_unregister_driver(&mcp3901_driver);
}
module_exit(mcp3901_exit);

MODULE_AUTHOR("Frank Maker <frank.maker@gmail.com>");
MODULE_DESCRIPTION("MCP3901 ADC SPI Driver");
MODULE_LICENSE("GPL");

