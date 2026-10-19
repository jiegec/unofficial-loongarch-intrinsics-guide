// Writes vector channel data to the selected SIO stream. Stream storage, channel layout, and stage routing follow the graphics SIO ABI.
// [HW-observed] GS-carrier SIO write: the stream/channel routing and the
// encoded rotation are part of the SIO ABI; the CPU model passes the
// written channel data through (the immediate form has no register result).
dst[0] = src[0];
