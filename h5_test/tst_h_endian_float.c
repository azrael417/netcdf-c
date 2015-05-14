/* This is part of the netCDF package.
   Copyright 2005 University Corporation for Atmospheric Research/Unidata
   See COPYRIGHT file for conditions of use.

   Test HDF5 file code. These are not intended to be exhaustive tests,
   but they use HDF5 the same way that netCDF-4 does, so if these
   tests don't work, than netCDF-4 won't work either.

   This files tests dataset creation and writing.
*/

#include "h5_err_macros.h"
#include <hdf5.h>

#define FILE_NAME "tst_h_endian_float.h5"
#define GRP_NAME "Henry_V"
#define VAR_BOOL_NAME "Southhamptons_Battle_Record"
#define GRP2_NAME "Some_3D_Met_Data"
#define DIM1_LEN 3
#define MAX_DIMS 255

int
main()
{
  hid_t fileid, grpid, spaceid, datasetid;
  int bool_out[DIM1_LEN] = {0, 1, 0};
  hsize_t dims[1];

  printf("*** Checking HDF5 endianness control...");

#define NATIVE_VAR_NAME "native_var"
#define LE_VAR_NAME "le_var"
#define BE_VAR_NAME "be_var"
  #define LAT_LEN 2
#define LON_LEN 3
#define NDIMS 3
#define LAT_NAME "Lat"
#define LON_NAME "Lon"
#define TIME_NAME "Time"
#define PRES_NAME "Pressure"
#define TEMP_NAME "Temperature"
  {
    int data[DIM1_LEN], data_in[DIM1_LEN];
    hid_t typeid, native_typeid;
    hid_t native_did, le_did, be_did;
    H5T_order_t order;
    htri_t equal;
    int i;
    float float_data_out[LAT_LEN][LON_LEN];
    int lat, lon;

   for (lat = 0; lat < LAT_LEN; lat++)
	 for (lon = 0; lon < LON_LEN; lon++)
	    float_data_out[lat][lon] = -666.666;

    /* Open file and create group. */
    if ((fileid = H5Fcreate(FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT,
                            H5P_DEFAULT)) < 0) ERR;
    if ((grpid = H5Gcreate(fileid, GRP_NAME, 0)) < 0) ERR;

    /* Create a dataset of native endian. */
    dims[0] = DIM1_LEN;
    if ((spaceid = H5Screate_simple(1, dims, dims)) < 0) ERR;
    if ((native_did = H5Dcreate(grpid, NATIVE_VAR_NAME, H5T_NATIVE_FLOAT,
                                spaceid, H5P_DEFAULT)) < 0) ERR;
    if ((le_did = H5Dcreate(grpid, LE_VAR_NAME, H5T_IEEE_F32LE,
                            spaceid, H5P_DEFAULT)) < 0) ERR;
    if ((be_did = H5Dcreate(grpid, BE_VAR_NAME, H5T_IEEE_F32BE,
                            spaceid, H5P_DEFAULT)) < 0) ERR;

    if (H5Dwrite(native_did, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                 data) < 0) ERR;
    if (H5Dwrite(le_did, H5T_IEEE_F32LE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                 data) < 0) ERR;
    if (H5Dwrite(be_did, H5T_IEEE_F32BE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                 data) < 0) ERR;
    if (H5Dclose(native_did) < 0 ||
        H5Dclose(le_did) < 0 ||
        H5Dclose(be_did) < 0 ||
        H5Sclose(spaceid) < 0 ||
        H5Gclose(grpid) < 0 ||
        H5Fclose(fileid) < 0)
      ERR;

    /* Now reopen the file and check. */
    if ((fileid = H5Fopen(FILE_NAME, H5F_ACC_RDONLY, H5P_DEFAULT)) < 0) ERR;
    if ((grpid = H5Gopen(fileid, GRP_NAME)) < 0) ERR;

    if ((native_did = H5Dopen1(grpid, NATIVE_VAR_NAME)) < 0) ERR;
    if ((typeid = H5Dget_type(native_did)) < 0) ERR;
    if ((native_typeid = H5Tget_native_type(typeid, H5T_DIR_DESCEND)) < 0) ERR;
    if ((order = H5Tget_order(typeid)) < 0) ERR;
    if ((equal = H5Tequal(typeid, native_typeid)) < 0) ERR;
    if (!equal) ERR;

    if (H5Dread(native_did, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                data_in) < 0) ERR;
    for (i = 0; i < DIM1_LEN; i++)
      if (data[i] != data_in[i]) ERR;

    if ((le_did = H5Dopen1(grpid, LE_VAR_NAME)) < 0) ERR;
    if ((typeid = H5Dget_type(le_did)) < 0) ERR;
    if ((order = H5Tget_order(typeid)) < 0) ERR;
    if (order != H5T_ORDER_LE) ERR;

    if (H5Dread(le_did, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                data_in) < 0) ERR;
    for (i = 0; i < DIM1_LEN; i++)
      if (data[i] != data_in[i]) ERR;

    if ((be_did = H5Dopen1(grpid, BE_VAR_NAME)) < 0) ERR;
    if ((typeid = H5Dget_type(be_did)) < 0) ERR;
    if ((order = H5Tget_order(typeid)) < 0) ERR;
    if (order != H5T_ORDER_BE) ERR;

    if (H5Dread(be_did, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                data_in) < 0) ERR;
    for (i = 0; i < DIM1_LEN; i++)
      if (data[i] != data_in[i]) ERR;

    if (H5Dclose(native_did) < 0 ||
        H5Dclose(le_did) < 0 ||
        H5Dclose(be_did) < 0 ||
        H5Gclose(grpid) < 0 ||
        H5Fclose(fileid) < 0)
      ERR;
  }

  SUMMARIZE_ERR;
  FINAL_RESULTS;
}
