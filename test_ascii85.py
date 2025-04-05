import subprocess
import base64

def run_encoder(input_data, adobe=False):
    cmd = ["./ascii85", "-e"]
    if adobe:
        cmd.append("--adobe")
    return subprocess.run(
        cmd,
        input=input_data,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )

def run_decoder(input_data, adobe=False):
    cmd = ["./ascii85", "-d"]
    if adobe:
        cmd.append("--adobe")
    return subprocess.run(
        cmd,
        input=input_data,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )

def test_encoder():
    # Test empty input
    input_data = b""
    result = run_encoder(input_data)
    assert result.returncode == 0
    expected = base64.a85encode(input_data, adobe=False)
    assert result.stdout == expected

    # Test Adobe mode
    input_data = b"Hello"
    result = run_encoder(input_data, adobe=True)
    assert result.returncode == 0
    expected = base64.a85encode(input_data, adobe=True)
    assert result.stdout == expected

def test_decoder():
    # Test valid decoding
    original = b"Test data"
    encoded = base64.a85encode(original, adobe=False)
    result = run_decoder(encoded)
    assert result.returncode == 0
    assert result.stdout == original

if __name__ == "__main__":
    test_encoder()
    test_decoder()
    print("All tests passed!")