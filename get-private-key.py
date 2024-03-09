from substrateinterface import KeypairType
from robonomicsinterface import Account

seed = input("Write your account seed phrase: ")
acc = Account(seed, crypto_type=KeypairType.ED25519)
print(f"Private key: {acc.keypair.private_key.hex()}")
print(f"SS58 address: {acc.get_address()}")