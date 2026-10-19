// Reads channel data from the selected SIO stream using the encoded rotation. The form uses an explicit stream identifier, a four-channel transfer. Stream storage, channel layout, and stage routing follow the graphics SIO ABI.
// [HW-observed] GS-carrier SIO read: the stream/channel selection and the
// encoded rotation are part of the SIO ABI; the CPU model passes the read
// channel data through.
dst[0] = src[0];
