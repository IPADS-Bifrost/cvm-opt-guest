// SPDX-License-Identifier: GPL-2.0-or-later
/*
   drm_edid_load.c: use a built-in EDID data set or load it via the firmware
		    interface

   Copyright (C) 2012 Carsten Emde <C.Emde@osadl.org>

*/

#include <linux/firmware.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_edid.h>
#include <drm/drm_print.h>

static char edid_firmware[PATH_MAX];
module_param_string(edid_firmware, edid_firmware, sizeof(edid_firmware), 0644);
MODULE_PARM_DESC(edid_firmware, "Do not probe monitor, use specified EDID blob "
	"from built-in data or /lib/firmware instead. ");

/* Use only for backward compatibility with drm_kms_helper.edid_firmware */
int __drm_set_edid_firmware_path(const char *path)
{
	scnprintf(edid_firmware, sizeof(edid_firmware), "%s", path);

	return 0;
}
EXPORT_SYMBOL(__drm_set_edid_firmware_path);

/* Use only for backward compatibility with drm_kms_helper.edid_firmware */
int __drm_get_edid_firmware_path(char *buf, size_t bufsize)
{
	return scnprintf(buf, bufsize, "%s", edid_firmware);
}
EXPORT_SYMBOL(__drm_get_edid_firmware_path);

#define GENERIC_EDIDS 6
static const char * const generic_edid_name[GENERIC_EDIDS] = {
	"edid/800x600.bin",
	"edid/1024x768.bin",
	"edid/1280x1024.bin",
	"edid/1600x1200.bin",
	"edid/1680x1050.bin",
	"edid/1920x1080.bin",
};

static const u8 generic_edid[GENERIC_EDIDS][128] = {
	{
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x31, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x05, 0x16, 0x01, 0x03, 0x6d, 0x1b, 0x14, 0x78,
	0xea, 0x5e, 0xc0, 0xa4, 0x59, 0x4a, 0x98, 0x25,
	0x20, 0x50, 0x54, 0x01, 0x00, 0x00, 0x45, 0x40,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xa0, 0x0f,
	0x20, 0x00, 0x31, 0x58, 0x1c, 0x20, 0x28, 0x80,
	0x14, 0x00, 0x15, 0xd0, 0x10, 0x00, 0x00, 0x1e,
	0x00, 0x00, 0x00, 0xff, 0x00, 0x4c, 0x69, 0x6e,
	0x75, 0x78, 0x20, 0x23, 0x30, 0x0a, 0x20, 0x20,
	0x20, 0x20, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x3b,
	0x3d, 0x24, 0x26, 0x05, 0x00, 0x0a, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xfc,
	0x00, 0x4c, 0x69, 0x6e, 0x75, 0x78, 0x20, 0x53,
	0x56, 0x47, 0x41, 0x0a, 0x20, 0x20, 0x00, 0xc2,
	},
	{
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x31, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x05, 0x16, 0x01, 0x03, 0x6d, 0x23, 0x1a, 0x78,
	0xea, 0x5e, 0xc0, 0xa4, 0x59, 0x4a, 0x98, 0x25,
	0x20, 0x50, 0x54, 0x00, 0x08, 0x00, 0x61, 0x40,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x64, 0x19,
	0x00, 0x40, 0x41, 0x00, 0x26, 0x30, 0x08, 0x90,
	0x36, 0x00, 0x63, 0x0a, 0x11, 0x00, 0x00, 0x18,
	0x00, 0x00, 0x00, 0xff, 0x00, 0x4c, 0x69, 0x6e,
	0x75, 0x78, 0x20, 0x23, 0x30, 0x0a, 0x20, 0x20,
	0x20, 0x20, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x3b,
	0x3d, 0x2f, 0x31, 0x07, 0x00, 0x0a, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xfc,
	0x00, 0x4c, 0x69, 0x6e, 0x75, 0x78, 0x20, 0x58,
	0x47, 0x41, 0x0a, 0x20, 0x20, 0x20, 0x00, 0x55,
	},
	{
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x31, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x05, 0x16, 0x01, 0x03, 0x6d, 0x2c, 0x23, 0x78,
	0xea, 0x5e, 0xc0, 0xa4, 0x59, 0x4a, 0x98, 0x25,
	0x20, 0x50, 0x54, 0x00, 0x00, 0x00, 0x81, 0x80,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x30, 0x2a,
	0x00, 0x98, 0x51, 0x00, 0x2a, 0x40, 0x30, 0x70,
	0x13, 0x00, 0xbc, 0x63, 0x11, 0x00, 0x00, 0x1e,
	0x00, 0x00, 0x00, 0xff, 0x00, 0x4c, 0x69, 0x6e,
	0x75, 0x78, 0x20, 0x23, 0x30, 0x0a, 0x20, 0x20,
	0x20, 0x20, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x3b,
	0x3d, 0x3e, 0x40, 0x0b, 0x00, 0x0a, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xfc,
	0x00, 0x4c, 0x69, 0x6e, 0x75, 0x78, 0x20, 0x53,
	0x58, 0x47, 0x41, 0x0a, 0x20, 0x20, 0x00, 0xa0,
	},
	{
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x31, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x05, 0x16, 0x01, 0x03, 0x6d, 0x37, 0x29, 0x78,
	0xea, 0x5e, 0xc0, 0xa4, 0x59, 0x4a, 0x98, 0x25,
	0x20, 0x50, 0x54, 0x00, 0x00, 0x00, 0xa9, 0x40,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x48, 0x3f,
	0x40, 0x30, 0x62, 0xb0, 0x32, 0x40, 0x40, 0xc0,
	0x13, 0x00, 0x2b, 0xa0, 0x21, 0x00, 0x00, 0x1e,
	0x00, 0x00, 0x00, 0xff, 0x00, 0x4c, 0x69, 0x6e,
	0x75, 0x78, 0x20, 0x23, 0x30, 0x0a, 0x20, 0x20,
	0x20, 0x20, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x3b,
	0x3d, 0x4a, 0x4c, 0x11, 0x00, 0x0a, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xfc,
	0x00, 0x4c, 0x69, 0x6e, 0x75, 0x78, 0x20, 0x55,
	0x58, 0x47, 0x41, 0x0a, 0x20, 0x20, 0x00, 0x9d,
	},
	{
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x31, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x05, 0x16, 0x01, 0x03, 0x6d, 0x2b, 0x1b, 0x78,
	0xea, 0x5e, 0xc0, 0xa4, 0x59, 0x4a, 0x98, 0x25,
	0x20, 0x50, 0x54, 0x00, 0x00, 0x00, 0xb3, 0x00,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x21, 0x39,
	0x90, 0x30, 0x62, 0x1a, 0x27, 0x40, 0x68, 0xb0,
	0x36, 0x00, 0xb5, 0x11, 0x11, 0x00, 0x00, 0x1e,
	0x00, 0x00, 0x00, 0xff, 0x00, 0x4c, 0x69, 0x6e,
	0x75, 0x78, 0x20, 0x23, 0x30, 0x0a, 0x20, 0x20,
	0x20, 0x20, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x3b,
	0x3d, 0x40, 0x42, 0x0f, 0x00, 0x0a, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xfc,
	0x00, 0x4c, 0x69, 0x6e, 0x75, 0x78, 0x20, 0x57,
	0x53, 0x58, 0x47, 0x41, 0x0a, 0x20, 0x00, 0x26,
	},
	{
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x31, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x05, 0x16, 0x01, 0x03, 0x6d, 0x32, 0x1c, 0x78,
	0xea, 0x5e, 0xc0, 0xa4, 0x59, 0x4a, 0x98, 0x25,
	0x20, 0x50, 0x54, 0x00, 0x00, 0x00, 0xd1, 0xc0,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3a,
	0x80, 0x18, 0x71, 0x38, 0x2d, 0x40, 0x58, 0x2c,
	0x45, 0x00, 0xf4, 0x19, 0x11, 0x00, 0x00, 0x1e,
	0x00, 0x00, 0x00, 0xff, 0x00, 0x4c, 0x69, 0x6e,
	0x75, 0x78, 0x20, 0x23, 0x30, 0x0a, 0x20, 0x20,
	0x20, 0x20, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x3b,
	0x3d, 0x42, 0x44, 0x0f, 0x00, 0x0a, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xfc,
	0x00, 0x4c, 0x69, 0x6e, 0x75, 0x78, 0x20, 0x46,
	0x48, 0x44, 0x0a, 0x20, 0x20, 0x20, 0x00, 0x05,
	},
};

static int edid_size(const u8 *edid, int data_size)
{
	if (data_size < EDID_LENGTH)
		return 0;

	return (edid[0x7e] + 1) * EDID_LENGTH;
}

static void *edid_load(struct drm_connector *connector, const char *name,
			const char *connector_name)
{
	const struct firmware *fw = NULL;
	const u8 *fwdata;
	u8 *edid;
	int fwsize, builtin;
	int i, valid_extensions = 0;
	bool print_bad_edid = !connector->bad_edid_counter || drm_debug_enabled(DRM_UT_KMS);

	builtin = match_string(generic_edid_name, GENERIC_EDIDS, name);
	if (builtin >= 0) {
		fwdata = generic_edid[builtin];
		fwsize = sizeof(generic_edid[builtin]);
	} else {
		struct platform_device *pdev;
		int err;

		pdev = platform_device_register_simple(connector_name, -1, NULL, 0);
		if (IS_ERR(pdev)) {
			DRM_ERROR("Failed to register EDID firmware platform device "
				  "for connector \"%s\"\n", connector_name);
			return ERR_CAST(pdev);
		}

		err = request_firmware(&fw, name, &pdev->dev);
		platform_device_unregister(pdev);
		if (err) {
			DRM_ERROR("Requesting EDID firmware \"%s\" failed (err=%d)\n",
				  name, err);
			return ERR_PTR(err);
		}

		fwdata = fw->data;
		fwsize = fw->size;
	}

	if (edid_size(fwdata, fwsize) != fwsize) {
		DRM_ERROR("Size of EDID firmware \"%s\" is invalid "
			  "(expected %d, got %d\n", name,
			  edid_size(fwdata, fwsize), (int)fwsize);
		edid = ERR_PTR(-EINVAL);
		goto out;
	}

	edid = kmemdup(fwdata, fwsize, GFP_KERNEL);
	if (edid == NULL) {
		edid = ERR_PTR(-ENOMEM);
		goto out;
	}

	if (!drm_edid_block_valid(edid, 0, print_bad_edid,
				  &connector->edid_corrupt)) {
		connector->bad_edid_counter++;
		DRM_ERROR("Base block of EDID firmware \"%s\" is invalid ",
		    name);
		kfree(edid);
		edid = ERR_PTR(-EINVAL);
		goto out;
	}

	for (i = 1; i <= edid[0x7e]; i++) {
		if (i != valid_extensions + 1)
			memcpy(edid + (valid_extensions + 1) * EDID_LENGTH,
			    edid + i * EDID_LENGTH, EDID_LENGTH);
		if (drm_edid_block_valid(edid + i * EDID_LENGTH, i,
					 print_bad_edid,
					 NULL))
			valid_extensions++;
	}

	if (valid_extensions != edid[0x7e]) {
		u8 *new_edid;

		edid[EDID_LENGTH-1] += edid[0x7e] - valid_extensions;
		DRM_INFO("Found %d valid extensions instead of %d in EDID data "
		    "\"%s\" for connector \"%s\"\n", valid_extensions,
		    edid[0x7e], name, connector_name);
		edid[0x7e] = valid_extensions;

		new_edid = krealloc(edid, (valid_extensions + 1) * EDID_LENGTH,
				    GFP_KERNEL);
		if (new_edid)
			edid = new_edid;
	}

	DRM_INFO("Got %s EDID base block and %d extension%s from "
	    "\"%s\" for connector \"%s\"\n", (builtin >= 0) ? "built-in" :
	    "external", valid_extensions, valid_extensions == 1 ? "" : "s",
	    name, connector_name);

out:
	release_firmware(fw);
	return edid;
}

struct edid *drm_load_edid_firmware(struct drm_connector *connector)
{
	const char *connector_name = connector->name;
	char *edidname, *last, *colon, *fwstr, *edidstr, *fallback = NULL;
	struct edid *edid;

	if (edid_firmware[0] == '\0')
		return ERR_PTR(-ENOENT);

	/*
	 * If there are multiple edid files specified and separated
	 * by commas, search through the list looking for one that
	 * matches the connector.
	 *
	 * If there's one or more that doesn't specify a connector, keep
	 * the last one found one as a fallback.
	 */
	fwstr = kstrdup(edid_firmware, GFP_KERNEL);
	if (!fwstr)
		return ERR_PTR(-ENOMEM);
	edidstr = fwstr;

	while ((edidname = strsep(&edidstr, ","))) {
		colon = strchr(edidname, ':');
		if (colon != NULL) {
			if (strncmp(connector_name, edidname, colon - edidname))
				continue;
			edidname = colon + 1;
			break;
		}

		if (*edidname != '\0') /* corner case: multiple ',' */
			fallback = edidname;
	}

	if (!edidname) {
		if (!fallback) {
			kfree(fwstr);
			return ERR_PTR(-ENOENT);
		}
		edidname = fallback;
	}

	last = edidname + strlen(edidname) - 1;
	if (*last == '\n')
		*last = '\0';

	edid = edid_load(connector, edidname, connector_name);
	kfree(fwstr);

	return edid;
}
