# Initial MVP Feature Plan

## 1. TCP Connection Support
- connect
- disconnect
- reconnect
- connection status

---

## 2. UDP Support
- send/receive UDP packets
- maintain target information internally
- configurable target host and port

---

## 3. Data Sending
Support sending:
- hexadecimal data
- plain text
- binary files

---

## 4. Receive & Validate Responses
Features:
- receive raw responses
- display responses in hex/ascii
- validate responses automatically

Validation types:
- exact match
- prefix match
- contains match
- timeout checks

---

## 5. Server Runner
Features:
- automatically start server
- capture stdout/stderr
- detect crashes
- terminate server after tests complete

---

## 6. YAML Test Configurations
Features:
- reusable automated tests
- sequential execution
- configurable test scenarios

# Initial Goals
- eliminate throwaway testing clients
- simplify raw TCP/UDP testing
- improve debugging workflow
- provide reusable protocol tests
- support binary protocol development
