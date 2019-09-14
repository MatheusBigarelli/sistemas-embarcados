import os
import math
# import matplotlib.pyplot as plt

from primes import primes







def split_in_half_words(msg):
    half_words = []
    i = 0
    while i < len(msg):
        msb = msg[i+1]
        lsb = msg[i]
        half_words.append(msb + lsb)
        i += 2

    return half_words



def decode_half_word(half_word, key_index, even):
    if even:
        factor = primes[key_index] - primes[key_index - 1]
    else:
        factor = primes[key_index] - primes[key_index + 1]

    value = int(half_word, 16)
    if not even:
        value = 2**15 - (int(value)%2**15)
        
    value /= factor
    value = abs(int(value))

    decoded_hw = hex(value).replace('0x', '')
    decoded_hw = '0' * ( 4-len(decoded_hw) ) + decoded_hw
    
    return decoded_hw

def decode(msg, key_index):
    """ Gets the the list of the message bytes ([ff, 55, 06, ...])
    and iterates over it converting to a new list with the bytes decoded
    :param str msg: the list of the bytes
    """
    global primes
    i = 0
    even = True
    decoded_msg = []
    half_words = split_in_half_words(msg)
    for half_word in half_words:
        decoded_hw = decode_half_word(half_word, key_index, even)
        decoded_msg.append(decoded_hw)
        even = not even

    return decoded_msg




def get_message(msg_file_name):
    msg = ""
    with open(msg_file_name) as msg_file:
        msg += msg_file.read()
        msg_file.close()

    return msg


def to_string(decoded_msg):
    ascii_string = ""
    for half_word in decoded_msg:
        try:
            code0 = int(half_word[0:2], 16)
            code1 = int(half_word[2:], 16)

            if code0 < 0x30 or code0 > 0x7E:
                ascii_string += '.'
            else:
                ascii_string += bytes.fromhex(half_word[0:2]).decode('utf-8')

            if code1 < 0x20 or code1 > 0x7E:
                ascii_string += '.'
            else:
                ascii_string += bytes.fromhex(half_word[2:]).decode('utf-8')
            
        except:
            pass

    return ascii_string


def get_next_key(count, algorithm):
    # Linear search
    if algorithm == 1:
        return count, primes[count]
        
    if algorithm == 2:
        return count, primes[count]

if __name__ == "__main__":
    # Creating 2D matrix for alg statistics
    data = []

    for i in range(1,4):
        msg_file_name = 'msg' + str(i) + '.txt'

        msg = get_message(msg_file_name)
        msg_bytes = msg.replace('\n', ' ').split(' ')

        last_byte = len(msg_bytes) - 8 # Takes out 2 last words
        confirmation_second_to_last = msg_bytes[last_byte:last_byte+4]
        confirmation_second_to_last = confirmation_second_to_last[::-1]
        confirmation_second_to_last = "".join(confirmation_second_to_last)
        expected_conf_sec_to_last = int(confirmation_second_to_last, 16)
        
        confirmation_last = msg_bytes[last_byte+4:]
        confirmation_last = confirmation_last[::-1]
        confirmation_last = "".join(confirmation_last)
        expected_conf_last = int(confirmation_last, 16)

        msg_bytes = msg_bytes[:last_byte]
        
        # for key_index, key in enumerate(primes):
        count = 1
        while True:
            try:
                key_index, key = get_next_key(count, 1)
            except Exception as e:
                print(e)
                print(count)
                break
            count += 1
            decoded_msg = decode(msg_bytes, key_index)
            calc_conf_sec_to_last = primes[key_index] * primes[key_index+1]
            calc_conf_last = primes[key_index] * primes[key_index-1]
            if  (calc_conf_sec_to_last == expected_conf_sec_to_last and  
                    calc_conf_last == expected_conf_last):
                os.system('cls')
                print("Decrypted message " + str(i))
                print("Key: {}".format(key))
                print("Decoded message: " + " ".join(decoded_msg))
                print("Decoded string: " + to_string(decoded_msg))
                print('\n\n')
                
                # print("Expected conf: " + hex(expected_conf_sec_to_last))
                # print("Calculated conf: " + hex(calc_conf_sec_to_last))
                # print('ok')
                garbage = input()
                data.append(count)
                break

    # plt.plot()
    # plt.show()