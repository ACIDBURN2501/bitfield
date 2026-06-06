# Security Policy

## Reporting a Vulnerability

Use GitHub's Security Advisories feature to report security concerns
privately.

Expect a response within 7 days. If the issue is confirmed, a fix
will be released as a patch version.

## Scope

bitfield is a small bit manipulation library with no network stack, no
external dependencies, and no dynamic memory allocation. The primary
attack surface is integer overflow in range/shift calculations and
out-of-bounds bit position access.
