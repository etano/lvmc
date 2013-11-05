#ifndef IO_H
#define IO_H

#include "../config.h"
#include "IO_Datatype.h"

class IOClass
{
public:
  /// Gets for HDF5 traits
  // Get type
  template <class T>
  inline H5::PredType GetHDF5Datatype(T &val) { return IO::hdf5_type_traits<T>::get_type(val); }
  // Get address of first element
  template <class T>
  inline void* GetHDF5Addr(T &val) { return IO::hdf5_type_traits<T>::get_addr(val); }
  // Get size of whole object
  template <class T>
  inline size_t GetHDF5Size(T &val) { return IO::hdf5_type_traits<T>::get_size(val); }
  // Get constant pointer to shape array
  template <class T>
  inline const hsize_t* GetHDF5Shape(T &val) { return IO::hdf5_type_traits<T>::get_shape(val); }
  // Get rank of object
  template <class T>
  inline const int GetHDF5Rank(T &val) { return IO::hdf5_type_traits<T>::get_rank(val); }

  // Read
  template<class T>
  inline void Read(H5::H5File file, const std::string& data_set_name, T& data)
  {
    H5::DataSet dataset = file.openDataSet(data_set_name);
    H5::DataSpace dataspace = dataset.getSpace();
    H5::PredType datatype = GetHDF5Datatype(data);
    dataset.read(GetHDF5Addr(data), datatype, dataspace, dataspace);
  }

  // Write
  template<class T>
  inline void Write(H5::H5File file, const std::string& data_set_name, T& data)
  {
    H5::PredType datatype = GetHDF5Datatype(data);
    datatype.setOrder(H5T_ORDER_LE);
    H5::DataSpace dataspace(GetHDF5Rank(data), GetHDF5Shape(data));
    H5::DataSet dataset = file.createDataSet(data_set_name, datatype, dataspace);
    dataset.write(GetHDF5Addr(data), datatype);
  }

};
#endif
