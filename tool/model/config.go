// Adapted from https://github.com/mongoose-os/mos/blob/master/mos/atca/config.go
// with major modifications made to fully support ATECC608A configuration.

// Copyright (c) 2014-2019 Cesanta Software Limited
// All rights reserved. Licensed under the Apache License, Version 2.0

package model

import (
	"bytes"
	"encoding/binary"
	"errors"
)

const (
	ConfigSize = 128
	HeaderSize = 16
)

type LockZone int

const (
	LockZoneConfig LockZone = 0
	LockZoneData            = 1
)

type Config struct {

	// First 16 bytes - skipped when writing config
	SerialNum  []byte `json:",omitempty" yaml:",omitempty"`
	Revision   uint32 `json:",omitempty" yaml:",omitempty"`
	AesEnable  bool   `json:",omitempty" yaml:",omitempty"`
	I2CEnable  bool   `json:",omitempty" yaml:",omitempty"`
	Reserved15 uint8  `json:",omitempty" yaml:",omitempty"`

	// Remainder
	I2CAddress            uint8
	Reserved17            uint8
	CountMatch            CountMatch
	ChipMode              ChipMode
	SlotInfo              []SlotInfo
	Counter0              uint64
	Counter1              uint64
	UseLock               UseLock
	VolatileKeyPermission VolatileKeyPermission
	SecureBoot            SecureBoot
	KdfIvLoc              uint8
	KdfIvStr              uint16
	Reserved68            uint8
	Reserved68b           uint64
	UserExtra             uint8
	UserExtraAdd          uint8
	LockValue             LockMode
	LockConfig            LockMode
	SlotLocked            uint16
	ChipOptions           ChipOptions
	X509Format            []X509Format
}

type CountMatch struct {
	CountMatchEnable bool
	Reserved         uint8
	CountMatchKey    uint8
}

type ChipMode struct {
	SelectorWriteOnce bool
	TTLEnable         bool
	WatchDogDuration  WatchdogDuration
	ClockDivider      uint8
}

type WatchdogDuration string

const (
	Watchdog1  WatchdogDuration = "1s"
	Watchdog10                  = "10s"
)

// This struct is not stored in the chip and simply contains
// both SlotConfig and KeyConfig together, for convenience and readability.
type SlotInfo struct {
	Num        uint8
	SlotConfig SlotConfig
	KeyConfig  KeyConfig
}

type SlotConfig struct {
	PrivateKeySlotConfig *PrivateKeySlotConfig `json:",omitempty" yaml:",omitempty"`
	ReadKey              *uint8                `json:",omitempty" yaml:",omitempty"`
	NoMAC                bool
	LimitedUse           bool
	EncryptRead          bool
	IsSecret             bool
	WriteKey             uint8
	WriteConfig          uint8
}

type PrivateKeySlotConfig struct {
	ExtSignEnable  bool
	IntSignEnable  bool
	ECDHEnable     bool
	ECDHToNextSlot bool
}

type UseLock struct {
	UseLockEnable uint8
	UseLockKey    uint8
}

type VolatileKeyPermission struct {
	VolatileKeyPermitSlot   uint8
	Reserved                uint8
	VolatileKeyPermitEnable bool
}

type SecureBoot struct {
	SecureBootMode             uint8
	SecureBootPersistentEnable bool
	SecureBootRandNonce        bool
	SecureBootSigDig           uint8
	SecureBootPubKey           uint8
	Reserved0                  bool
	Reserved1                  uint8
}

type LockMode string

const (
	LockModeLocked    LockMode = "Locked"
	LockModeUnlocked           = "Unlocked"
	LockModeUnchanged          = "Unchanged"
)

type X509Format struct {
	PublicPosition uint8
	TemplateLength uint8
}

type ChipOptions struct {
	PowerOnSelfTest       bool
	IoProtectionKeyEnable bool
	KdfAesEnable          bool
	AutoClearFirstFail    bool
	EcdhProtectionBits    uint8
	KdfProtectionBits     uint8
	IoProtectionKey       uint8
}

type KeyConfig struct {
	Private          bool    // 0
	PubInfo          bool    // 1
	KeyType          KeyType // 2, 3, 4
	Lockable         bool    // 5
	ReqRandom        bool    // 6
	ReqAuth          bool    // 7
	AuthKey          uint8   // 8,9,10,11
	IntrusionDisable bool    // 12
	// 13 - Reserved
	X509ID uint8 // 14,15
}

type KeyType string

const (
	KeyTypeECC KeyType = "ECC"    // 0x4 = P256 NIST ECC key
	KeyTypeAES         = "AES"    // 0x6 = AES key
	KeyTypeSHA         = "NonECC" // 0x7 = SHA key or other data
)

func parseSlotConfig(num int, scv uint16, kc *KeyConfig) SlotConfig {
	sc := SlotConfig{}
	if kc.Private {
		pkc := &PrivateKeySlotConfig{}
		pkc.ExtSignEnable = (scv&1 != 0)
		pkc.IntSignEnable = (scv&2 != 0)
		pkc.ECDHEnable = (scv&4 != 0)
		pkc.ECDHToNextSlot = (scv&8 != 0)
		sc.PrivateKeySlotConfig = pkc
	} else {
		rk := uint8(scv & 0xF)
		sc.ReadKey = &rk
	}
	sc.NoMAC = (scv&0x10 != 0)
	sc.LimitedUse = (scv&0x20 != 0)
	sc.EncryptRead = (scv&0x40 != 0)
	sc.IsSecret = (scv&0x80 != 0)
	sc.WriteKey = uint8((scv >> 8) & 0xF)
	sc.WriteConfig = uint8((scv >> 12) & 0xF)
	return sc
}

func parseLockMode(b uint8) (LockMode, error) {
	if b == 0x55 {
		return LockModeUnlocked, nil
	} else if b == 0x00 {
		return LockModeLocked, nil
	} else if b == 0xff {
		return LockModeUnchanged, nil
	} else {
		return "", errors.New("unknown data lock mode")
	}
}

func parseKeyConfig(num int, kcv uint16) (*KeyConfig, error) {
	var err error
	kc := &KeyConfig{}
	kc.Private = (kcv&1 != 0)
	kc.PubInfo = (kcv&2 != 0)
	kc.KeyType, err = parseKeyType(uint8((kcv >> 2) & 0x7))
	if err != nil {
		return nil, err
	}
	kc.Lockable = (kcv&0x20 != 0)
	kc.ReqRandom = (kcv&0x40 != 0)
	kc.ReqAuth = (kcv&0x80 != 0)
	kc.AuthKey = uint8((kcv >> 8) & 0xF)
	kc.IntrusionDisable = (kcv&0x1000 != 0)
	kc.X509ID = uint8((kcv >> 14) & 0x3)
	return kc, nil
}

func parseKeyType(b uint8) (KeyType, error) {
	if b == 4 {
		return KeyTypeECC, nil
	} else if b == 6 {
		return KeyTypeAES, nil
	} else if b == 7 {
		return KeyTypeSHA, nil
	} else {
		return "", errors.New("unknown key type")
	}
}

func ParseBinaryConfig(cd []byte, includeHeader bool) (*Config, error) {
	cb := bytes.NewBuffer(cd)
	var b uint8
	var err error
	size := ConfigSize
	if !includeHeader {
		size -= HeaderSize
	}
	if len(cd) != size {
		return nil, errors.New("unexpected number of bytes")
	}
	c := &Config{}
	if includeHeader {
		c.SerialNum = make([]byte, 9)
		cb.Read(c.SerialNum[0:4])
		binary.Read(cb, binary.BigEndian, &c.Revision)
		cb.Read(c.SerialNum[4:9])
		binary.Read(cb, binary.BigEndian, &b)
		c.AesEnable = (b&1 != 0)
		binary.Read(cb, binary.BigEndian, &b)
		c.I2CEnable = (b&1 != 0)
		binary.Read(cb, binary.BigEndian, &c.Reserved15)
	}
	binary.Read(cb, binary.BigEndian, &c.I2CAddress)
	binary.Read(cb, binary.BigEndian, &c.Reserved17)
	binary.Read(cb, binary.BigEndian, &b)
	c.CountMatch.CountMatchEnable = (b&1 != 0)
	c.CountMatch.Reserved = ((b >> 1) & 7)
	c.CountMatch.CountMatchKey = ((b >> 4) & 0xF)
	binary.Read(cb, binary.BigEndian, &b)
	c.ChipMode.SelectorWriteOnce = (b&1 != 0)
	c.ChipMode.TTLEnable = (b&2 != 0)
	if b&4 != 0 {
		c.ChipMode.WatchDogDuration = Watchdog10
	} else {
		c.ChipMode.WatchDogDuration = Watchdog1
	}
	c.ChipMode.ClockDivider = ((b >> 5) & 0x1F)
	var scvs [16]uint16
	for i := 0; i < 16; i++ {
		binary.Read(cb, binary.LittleEndian, &scvs[i])
	}
	binary.Read(cb, binary.BigEndian, &c.Counter0)
	binary.Read(cb, binary.BigEndian, &c.Counter1)
	binary.Read(cb, binary.BigEndian, &b)
	c.UseLock.UseLockEnable = (b & 0xF)
	c.UseLock.UseLockKey = ((b >> 4) & 0xF)
	binary.Read(cb, binary.BigEndian, &b)
	c.VolatileKeyPermission.VolatileKeyPermitSlot = (b & 0xF)
	c.VolatileKeyPermission.Reserved = ((b >> 3) & 7)
	c.VolatileKeyPermission.VolatileKeyPermitEnable = (b&0x80 != 0)
	binary.Read(cb, binary.BigEndian, &b)
	c.SecureBoot.SecureBootMode = (b & 3)
	c.SecureBoot.Reserved0 = (b&4 != 0)
	c.SecureBoot.SecureBootPersistentEnable = (b&8 != 0)
	c.SecureBoot.SecureBootRandNonce = (b&16 != 0)
	c.SecureBoot.Reserved1 = ((b >> 5) & 7)
	binary.Read(cb, binary.BigEndian, &b)
	c.SecureBoot.SecureBootSigDig = (b & 0xF)
	c.SecureBoot.SecureBootPubKey = ((b >> 4) & 0xF)
	binary.Read(cb, binary.BigEndian, &c.KdfIvLoc)
	binary.Read(cb, binary.BigEndian, &c.KdfIvStr)
	binary.Read(cb, binary.BigEndian, &c.Reserved68)
	binary.Read(cb, binary.BigEndian, &c.Reserved68b)
	binary.Read(cb, binary.BigEndian, &c.UserExtra)
	binary.Read(cb, binary.BigEndian, &c.UserExtraAdd)
	binary.Read(cb, binary.BigEndian, &b)
	c.LockValue, err = parseLockMode(b)
	if err != nil {
		return nil, err
	}
	binary.Read(cb, binary.BigEndian, &b)
	c.LockConfig, err = parseLockMode(b)
	if err != nil {
		return nil, err
	}
	binary.Read(cb, binary.LittleEndian, &c.SlotLocked)
	binary.Read(cb, binary.BigEndian, &b)
	c.ChipOptions.PowerOnSelfTest = (b&1 != 0)
	c.ChipOptions.IoProtectionKeyEnable = (b&2 != 0)
	c.ChipOptions.KdfAesEnable = (b&4 != 0)
	c.ChipOptions.AutoClearFirstFail = (b&8 != 0)
	binary.Read(cb, binary.BigEndian, &b)
	c.ChipOptions.EcdhProtectionBits = (b & 3)
	c.ChipOptions.KdfProtectionBits = ((b >> 2) & 3)
	c.ChipOptions.IoProtectionKey = ((b >> 4) & 0xF)
	for i := 0; i < 4; i++ {
		var fc X509Format
		binary.Read(cb, binary.BigEndian, &b)
		fc.PublicPosition = (b & 0xF)
		fc.TemplateLength = ((b >> 4) & 0xF)
		c.X509Format = append(c.X509Format, fc)
	}
	for i := 0; i < 16; i++ {
		var kcv uint16
		binary.Read(cb, binary.LittleEndian, &kcv)
		kc, err := parseKeyConfig(i, kcv)
		if err != nil {
			return nil, err
		}
		c.SlotInfo = append(c.SlotInfo, SlotInfo{
			Num:        uint8(i),
			SlotConfig: parseSlotConfig(i, scvs[i], kc),
			KeyConfig:  *kc,
		})
	}
	return c, nil
}

func writeSlotConfig(cb *bytes.Buffer, si SlotInfo) error {
	var scv uint16
	sc := &si.SlotConfig
	if si.KeyConfig.Private {
		pkc := sc.PrivateKeySlotConfig
		if pkc == nil {
			return errors.New("no PrivateKeyConfig")
		}
		if pkc.ExtSignEnable {
			scv |= 1
		}
		if pkc.IntSignEnable {
			scv |= 2
		}
		if pkc.ECDHEnable {
			scv |= 4
		}
		if pkc.ECDHToNextSlot {
			scv |= 8
		}
	} else if si.Num < 16 {
		if sc.ReadKey == nil {
			return errors.New("no ReadKey")
		}
		scv = uint16(*sc.ReadKey)
	} else {
		return errors.New("invalid slot number")
	}
	if sc.NoMAC {
		scv |= 0x10
	}
	if sc.LimitedUse {
		scv |= 0x20
	}
	if sc.EncryptRead {
		scv |= 0x40
	}
	if sc.IsSecret {
		scv |= 0x80
	}
	scv |= (uint16(sc.WriteKey&0xF) << 8)
	scv |= (uint16(sc.WriteConfig&0xF) << 12)
	binary.Write(cb, binary.LittleEndian, scv)
	return nil
}

func writeKeyConfig(cb *bytes.Buffer, si SlotInfo) error {
	var kcv uint16
	kc := &si.KeyConfig
	if kc.Private {
		kcv |= 1
	}
	if kc.PubInfo {
		kcv |= 2
	}
	switch kc.KeyType {
	case KeyTypeECC:
		kcv |= (uint16(4) << 2)
	case KeyTypeAES:
		kcv |= (uint16(6) << 2)
	case KeyTypeSHA:
		kcv |= (uint16(7) << 2)
	default:
		return errors.New("unknown key type")
	}
	if kc.Lockable {
		kcv |= 0x20
	}
	if kc.ReqRandom {
		kcv |= 0x40
	}
	if kc.ReqAuth {
		kcv |= 0x80
	}
	kcv |= (uint16(kc.AuthKey&0xF) << 8)
	if kc.IntrusionDisable {
		kcv |= 0x1000
	}
	kcv |= (uint16(kc.X509ID&0x3) << 14)
	binary.Write(cb, binary.LittleEndian, kcv)
	return nil
}

func writeLockMode(cb *bytes.Buffer, lm LockMode) error {
	var b uint8
	switch lm {
	case LockModeUnlocked:
		b = 0x55
	case LockModeLocked:
		b = 0
	case LockModeUnchanged:
		b = 0xff
	default:
		return errors.New("unknown lock mode")
	}
	return binary.Write(cb, binary.BigEndian, b)
}

func WriteBinaryConfig(c *Config, includeHeader bool) ([]byte, error) {
	var b uint8
	var err error
	size := ConfigSize
	if !includeHeader {
		size -= HeaderSize
	}
	cd := make([]byte, 0, size)
	cb := bytes.NewBuffer(cd)
	if includeHeader {
		sn := c.SerialNum
		if sn == nil {
			sn = make([]byte, 9)
		}
		cb.Write(sn[0:4])
		binary.Write(cb, binary.BigEndian, c.Revision)
		cb.Write(sn[4:9])
		b = 0
		if c.AesEnable {
			b |= 1
		}
		binary.Write(cb, binary.BigEndian, b)
		b = 0
		if c.I2CEnable {
			b |= 1
		}
		binary.Write(cb, binary.BigEndian, b)
		binary.Write(cb, binary.BigEndian, c.Reserved15)
	}
	binary.Write(cb, binary.BigEndian, c.I2CAddress)
	binary.Write(cb, binary.BigEndian, c.Reserved17)
	b = 0
	if c.CountMatch.CountMatchEnable {
		b |= 1
	}
	b |= ((c.CountMatch.Reserved & 7) << 1)
	b |= ((c.CountMatch.CountMatchKey & 0xF) << 4)
	binary.Write(cb, binary.BigEndian, b)
	b = 0
	if c.ChipMode.SelectorWriteOnce {
		b |= 1
	}
	if c.ChipMode.TTLEnable {
		b |= 2
	}
	switch c.ChipMode.WatchDogDuration {
	case Watchdog1:
		break
	case Watchdog10:
		b |= 4
	default:
		return nil, errors.New("unknown watchdog duration")
	}
	b |= ((c.ChipMode.ClockDivider & 0x1F) << 5)
	binary.Write(cb, binary.BigEndian, b)
	for i := 0; i < 16; i++ {
		err = writeSlotConfig(cb, c.SlotInfo[i])
		if err != nil {
			return nil, err
		}
	}
	binary.Write(cb, binary.BigEndian, c.Counter0)
	binary.Write(cb, binary.BigEndian, c.Counter1)
	b = 0
	b |= (c.UseLock.UseLockEnable & 0xF)
	b |= ((c.UseLock.UseLockKey & 0xF) << 4)
	binary.Write(cb, binary.BigEndian, b)
	b = 0
	b |= (c.VolatileKeyPermission.VolatileKeyPermitSlot & 0xF)
	b |= ((c.VolatileKeyPermission.Reserved & 7) << 3)
	if c.VolatileKeyPermission.VolatileKeyPermitEnable {
		b |= 0x80
	}
	binary.Write(cb, binary.BigEndian, b)
	b = 0
	b |= (c.SecureBoot.SecureBootMode & 3)
	if c.SecureBoot.Reserved0 {
		b |= 4
	}
	if c.SecureBoot.SecureBootPersistentEnable {
		b |= 8
	}
	if c.SecureBoot.SecureBootRandNonce {
		b |= 16
	}
	b |= ((c.SecureBoot.Reserved1 & 7) << 5)
	binary.Write(cb, binary.BigEndian, b)
	b = 0
	b |= (c.SecureBoot.SecureBootSigDig & 0xF)
	b |= ((c.SecureBoot.SecureBootPubKey & 0xF) << 4)
	binary.Write(cb, binary.BigEndian, b)
	binary.Write(cb, binary.BigEndian, &c.KdfIvLoc)
	binary.Write(cb, binary.BigEndian, &c.KdfIvStr)
	binary.Write(cb, binary.BigEndian, &c.Reserved68)
	binary.Write(cb, binary.BigEndian, c.Reserved68b)
	binary.Write(cb, binary.BigEndian, c.UserExtra)
	binary.Write(cb, binary.BigEndian, c.UserExtraAdd)
	err = writeLockMode(cb, c.LockValue)
	if err != nil {
		return nil, err
	}
	err = writeLockMode(cb, c.LockConfig)
	if err != nil {
		return nil, err
	}
	binary.Write(cb, binary.LittleEndian, c.SlotLocked)
	b = 0
	if c.ChipOptions.PowerOnSelfTest {
		b |= 1
	}
	if c.ChipOptions.IoProtectionKeyEnable {
		b |= 2
	}
	if c.ChipOptions.KdfAesEnable {
		b |= 4
	}
	if c.ChipOptions.AutoClearFirstFail {
		b |= 8
	}
	binary.Write(cb, binary.BigEndian, b)
	b = 0
	b |= (c.ChipOptions.EcdhProtectionBits & 3)
	b |= ((c.ChipOptions.KdfProtectionBits & 3) << 2)
	b |= ((c.ChipOptions.IoProtectionKey & 0xF) << 4)
	binary.Write(cb, binary.BigEndian, b)
	for i := 0; i < 4; i++ {
		fc := &c.X509Format[i]
		b = 0
		b |= (fc.PublicPosition & 0xF)
		b |= ((fc.TemplateLength & 0xF) << 4)
		binary.Write(cb, binary.BigEndian, b)
	}
	for i := 0; i < 16; i++ {
		err = writeKeyConfig(cb, c.SlotInfo[i])
		if err != nil {
			return nil, err
		}
	}
	return cb.Bytes(), nil
}
