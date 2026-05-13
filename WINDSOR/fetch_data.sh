#!/usr/bin/env bash
#
# Fetch the computational mesh needed to run the WINDSOR case:
#   - MESH/c1g1.cgns  (~1.2 GB)  ~6.3 M cells
#
# The mesh is hosted outside the git repository because of its size.
# Once downloaded, the case directories are self-contained and can be
# run with the standard `code_saturne run` workflow.
#
# Usage:
#   ./fetch_data.sh
#
# The download base URL is configured via the WINDSOR_DATA_URL
# environment variable, or by editing the DATA_URL value below.

set -euo pipefail

DATA_URL="${WINDSOR_DATA_URL:-https://autocfdv3.s3.eu-west-1.amazonaws.com/test-cases/case1}"

cd "$(dirname "$0")"

fetch() {
    local remote_path="$1"
    local local_path="$2"
    local sha256="${3:-}"

    if [ -f "$local_path" ]; then
        echo "[skip] $local_path already present"
        return
    fi

    mkdir -p "$(dirname "$local_path")"
    echo "[get ] $local_path"
    curl -fL --progress-bar "$DATA_URL/$remote_path" -o "$local_path"

    if [ -n "$sha256" ]; then
        echo "[chk ] verifying sha256"
        echo "$sha256  $local_path" | sha256sum -c -
    fi
}

fetch "meshes/c1g1.cgns" "MESH/c1g1.cgns" \
      "15c55862c720a1110fe65579e20f6c7cfc506fc2da78c596d46145bd7d5d2bca"

echo "Done. You can now run:"
echo "  code_saturne run --case RANS"
echo "  code_saturne run --case DDES"
