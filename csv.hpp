#pragma once

#include <istream>
#include <ostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

using csv_row = std::vector<std::string>;
using csv = std::vector<csv_row>;

csv_row csv_read_row(std::istream &_file, char _delimiter) {
	std::stringstream ss;
	bool inquotes = false;
	csv_row row;

	while (_file.good()) {
		char c = static_cast<char>(_file.get());

		if (!inquotes && c == '"') {
			inquotes = true;
		}
		else if (inquotes && c == '"') {
			if (_file.peek() == '"') {
				ss << static_cast<char>(_file.get());
			}
			else {
				inquotes = false;
			}
		}
		else if (!inquotes && c == _delimiter) {
			row.push_back(ss.str());
			ss.str("");
		}
		else if (!inquotes && (c == '\r' || c == '\n')) {
			if (_file.peek() == '\n') {
				_file.get();
			}
			row.push_back(ss.str());
			return row;
		}
		else {
			ss << c;
		}
	}
	return row;
}

template <const char Delimiter = ','>
csv make_csv(std::istream &file) {
	csv table;
	while (file.good()) {
		csv_row row = csv_read_row(file, Delimiter);

		if (!row[0].find("#")) {
			continue;
		}
		else {
			table.push_back(row);
		}
	}

	return table;
}

std::ostream &operator<<(std::ostream &_os, const csv &_csv) {
	std::for_each(_csv.begin(), _csv.end(), [&_os](const csv_row &row) {
		std::for_each(row.begin(), row.end(), [&_os](const std::string &data) {
			_os << data << ',';
		});
		_os << '\n';
	});
	return _os;
}
