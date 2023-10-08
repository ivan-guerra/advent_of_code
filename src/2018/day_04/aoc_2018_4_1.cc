#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using SleepInterval = std::pair<int, int>;

struct DateTime {
  int year;
  int month;
  int day;
  int hour;
  int minute;
};

struct {
  bool operator()(const DateTime& a, const DateTime& b) const {
    if (a.year < b.year) {
      return true;
    } else if (a.year > b.year) {
      return false;
    }

    if (a.month < b.month) {
      return true;
    } else if (a.month > b.month) {
      return false;
    }

    if (a.day < b.day) {
      return true;
    } else if (a.day > b.day) {
      return false;
    }

    if (a.hour < b.hour) {
      return true;
    } else if (a.hour > b.hour) {
      return false;
    }

    if (a.minute < b.minute) {
      return true;
    } else if (a.minute > b.minute) {
      return false;
    }

    return true;
  }
} DateTimeLess;

struct LogEntry {
  DateTime datetime;
  std::string text;
};

struct {
  bool operator()(const LogEntry& a, const LogEntry& b) const {
    return (DateTimeLess(a.datetime, b.datetime));
  }
} LogEntryLess;

struct Guard {
  int id = 0;
  int asleep_time = 0;
  int wakeup_time = 0;
  int total_min_asleep = 0;
  std::vector<SleepInterval> sleep_intervals;
};

std::vector<std::string> GetWords(const std::string& text) {
  std::istringstream iss(text);
  std::vector<std::string> words;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(words));
  return words;
}

LogEntry ParseLogEntry(const std::string& text) {
  std::vector<std::string> words = GetWords(text);
  LogEntry entry;
  entry.datetime.year = std::stoi(words[0].substr(1, 4));
  entry.datetime.month = std::stoi(words[0].substr(6, 2));
  entry.datetime.day = std::stoi(words[0].substr(9, 2));
  entry.datetime.hour = std::stoi(words[1].substr(0, 2));
  entry.datetime.minute = std::stoi(words[1].substr(3, 2));

  for (int i = 2; i < words.size() - 1; ++i) {
    entry.text += words[i] + " ";
  }
  entry.text += words.back();

  return entry;
}

Guard FindGuardWithMostSleep(const std::vector<LogEntry>& logs) {
  std::unordered_map<int, Guard> guards;
  int guard_id = 0;
  for (const LogEntry& log : logs) {
    std::vector<std::string> words = GetWords(log.text);
    if (words[0] == "Guard") {
      guard_id = std::stoi(words[1].substr(1));
      guards[guard_id].id = guard_id;
    } else if (words[0] == "falls") {
      guards[guard_id].asleep_time = log.datetime.minute;
    } else if (words[0] == "wakes") {
      Guard& curr = guards[guard_id];

      curr.wakeup_time = log.datetime.minute;
      curr.total_min_asleep += curr.wakeup_time - curr.asleep_time;
      curr.sleep_intervals.emplace_back(curr.asleep_time, curr.wakeup_time);
    }
  }

  int max_sleep = 0;
  for (const auto& [gid, info] : guards) {
    if (info.total_min_asleep > max_sleep) {
      max_sleep = info.total_min_asleep;
      guard_id = gid;
    }
  }
  return guards[guard_id];
}

int FindSleepiestMinute(const Guard& guard) {
  std::vector<int> counts(60, 0);
  for (int i = 0; i < 60; ++i) {
    for (const SleepInterval& sint : guard.sleep_intervals) {
      if ((i >= sint.first) && (i < sint.second)) {
        counts[i]++;
      }
    }
  }

  int sleepiest_min = 0;
  int max_count = 0;
  for (int i = 0; i < counts.size(); ++i) {
    if (counts[i] > max_count) {
      sleepiest_min = i;
      max_count = counts[i];
    }
  }
  return sleepiest_min;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  std::vector<LogEntry> logs;
  while (std::getline(input, line)) {
    logs.push_back(ParseLogEntry(line));
  }

  std::sort(logs.begin(), logs.end(), LogEntryLess);
  Guard sleepiest_guard = FindGuardWithMostSleep(logs);
  std::cout << "Answer: "
            << sleepiest_guard.id * FindSleepiestMinute(sleepiest_guard)
            << std::endl;

  exit(EXIT_SUCCESS);
}
