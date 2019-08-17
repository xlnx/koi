#pragma once

#include <uv.h>

namespace koi
{
namespace uv
{
namespace err
{
enum struct Err : int
{
	ERR_2BIG = UV_E2BIG,
	/* argument list too long */
	ERR_ACCES = UV_EACCES,
	/* permission denied */
	ERR_ADDRINUSE = UV_EADDRINUSE,
	/* address already in use */
	ERR_ADDRNOTAVAIL = UV_EADDRNOTAVAIL,
	/* address not available */
	ERR_AFNOSUPPORT = UV_EAFNOSUPPORT,
	/* address family not supported */
	ERR_AGAIN = UV_EAGAIN,
	/* resource temporarily unavailable */
	ERR_AI_ADDRFAMILY = UV_EAI_ADDRFAMILY,
	/* address family not supported */
	ERR_AI_AGAIN = UV_EAI_AGAIN,
	/* temporary failure */
	ERR_AI_BADFLAGS = UV_EAI_BADFLAGS,
	/* bad ai_flags value */
	ERR_AI_BADHINTS = UV_EAI_BADHINTS,
	/* invalid value for hints */
	ERR_AI_CANCELED = UV_EAI_CANCELED,
	/* request canceled */
	ERR_AI_FAIL = UV_EAI_FAIL,
	/* permanent failure */
	ERR_AI_FAMILY = UV_EAI_FAMILY,
	/* ai_family not supported */
	ERR_AI_MEMORY = UV_EAI_MEMORY,
	/* out of memory */
	ERR_AI_NODATA = UV_EAI_NODATA,
	/* no address */
	ERR_AI_NONAME = UV_EAI_NONAME,
	/* unknown node or service */
	ERR_AI_OVERFLOW = UV_EAI_OVERFLOW,
	/* argument buffer overflow */
	ERR_AI_PROTOCOL = UV_EAI_PROTOCOL,
	/* resolved protocol is unknown */
	ERR_AI_SERVICE = UV_EAI_SERVICE,
	/* service not available for socket type */
	ERR_AI_SOCKTYPE = UV_EAI_SOCKTYPE,
	/* socket type not supported */
	ERR_ALREADY = UV_EALREADY,
	/* connection already in progress */
	ERR_BADF = UV_EBADF,
	/* bad file descriptor */
	ERR_BUSY = UV_EBUSY,
	/* resource busy or locked */
	ERR_CANCELED = UV_ECANCELED,
	/* operation canceled */
	ERR_CHARSET = UV_ECHARSET,
	/* invalid Unicode character */
	ERR_CONNABORTED = UV_ECONNABORTED,
	/* software caused connection abort */
	ERR_CONNREFUSED = UV_ECONNREFUSED,
	/* connection refused */
	ERR_CONNRESET = UV_ECONNRESET,
	/* connection reset by peer */
	ERR_DESTADDRREQ = UV_EDESTADDRREQ,
	/* destination address required */
	ERR_EXIST = UV_EEXIST,
	/* file already exists */
	ERR_FAULT = UV_EFAULT,
	/* bad address in system call argument */
	ERR_FBIG = UV_EFBIG,
	/* file too large */
	ERR_HOSTUNREACH = UV_EHOSTUNREACH,
	/* host is unreachable */
	ERR_INTR = UV_EINTR,
	/* interrupted system call */
	ERR_INVAL = UV_EINVAL,
	/* invalid argument */
	ERR_IO = UV_EIO,
	/* i/o error */
	ERR_ISCONN = UV_EISCONN,
	/* socket is already connected */
	ERR_ISDIR = UV_EISDIR,
	/* illegal operation on a directory */
	ERR_LOOP = UV_ELOOP,
	/* too many symbolic links encountered */
	ERR_MFILE = UV_EMFILE,
	/* too many open files */
	ERR_MSGSIZE = UV_EMSGSIZE,
	/* message too long */
	ERR_NAMETOOLONG = UV_ENAMETOOLONG,
	/* name too long */
	ERR_NETDOWN = UV_ENETDOWN,
	/* network is down */
	ERR_NETUNREACH = UV_ENETUNREACH,
	/* network is unreachable */
	ERR_NFILE = UV_ENFILE,
	/* file table overflow */
	ERR_NOBUFS = UV_ENOBUFS,
	/* no buffer space available */
	ERR_NODEV = UV_ENODEV,
	/* no such device */
	ERR_NOENT = UV_ENOENT,
	/* no such file or directory */
	ERR_NOMEM = UV_ENOMEM,
	/* not enough memory */
	ERR_NONET = UV_ENONET,
	/* machine is not on the network */
	ERR_NOPROTOOPT = UV_ENOPROTOOPT,
	/* protocol not available */
	ERR_NOSPC = UV_ENOSPC,
	/* no space left on device */
	ERR_NOSYS = UV_ENOSYS,
	/* function not implemented */
	ERR_NOTCONN = UV_ENOTCONN,
	/* socket is not connected */
	ERR_NOTDIR = UV_ENOTDIR,
	/* not a directory */
	ERR_NOTEMPTY = UV_ENOTEMPTY,
	/* directory not empty */
	ERR_NOTSOCK = UV_ENOTSOCK,
	/* socket operation on non-socket */
	ERR_NOTSUP = UV_ENOTSUP,
	/* operation not supported on socket */
	ERR_PERM = UV_EPERM,
	/* operation not permitted */
	ERR_PIPE = UV_EPIPE,
	/* broken pipe */
	ERR_PROTO = UV_EPROTO,
	/* protocol error */
	ERR_PROTONOSUPPORT = UV_EPROTONOSUPPORT,
	/* protocol not supported */
	ERR_PROTOTYPE = UV_EPROTOTYPE,
	/* protocol wrong type for socket */
	ERR_RANGE = UV_ERANGE,
	/* result too large */
	ERR_ROFS = UV_EROFS,
	/* read-only file system */
	ERR_SHUTDOWN = UV_ESHUTDOWN,
	/* cannot send after transport endpoint shutdown */
	ERR_SPIPE = UV_ESPIPE,
	/* invalid seek */
	ERR_SRCH = UV_ESRCH,
	/* no such process */
	ERR_TIMEDOUT = UV_ETIMEDOUT,
	/* connection timed out */
	ERR_TXTBSY = UV_ETXTBSY,
	/* text file is busy */
	ERR_XDEV = UV_EXDEV,
	/* cross-device link not permitted */
	ERR_UNKNOWN = UV_UNKNOWN,
	/* unknown error */
	ERR_EOF = UV_EOF,
	/* end of file */
	ERR_NXIO = UV_ENXIO,
	/* no such device or address */
	ERR_MLINK = UV_EMLINK,
	/* too many links */
};

struct Error final
{
	Error( int _ ) :
	  _( static_cast<Err>( _ ) ) {}
	Error( Err _ ) :
	  _( _ ) {}

	Err err() const { return _; }
	int err_code() const { return static_cast<int>( _ ); }
	const char *msg() const { return uv_strerror( static_cast<int>( _ ) ); }

private:
	Err _;
};

}  // namespace err

}  // namespace uv

}  // namespace koi