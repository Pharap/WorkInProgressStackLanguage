#pragma once

//
//   Copyright (C) 2018 Pharap (@Pharap)
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//

enum class ResultStatus
{
	Undefined,
	Success,
	Error,
};

class ResultInfo
{
private:
	ResultStatus status = ResultStatus::Undefined;
	const char * errorMessage = nullptr;

	ResultInfo(ResultStatus status)
		: status(status)
	{
	}

	ResultInfo(ResultStatus status, const char * errorMessage)
		: status(status), errorMessage(errorMessage)
	{
	}

	friend ResultInfo resultSuccess(void);
	friend ResultInfo resultError(const char * errorMessage);

public:	
	ResultInfo(void) = default;

	bool isError(void) const
	{
		return (this->status == ResultStatus::Error);
	}

	bool isSuccess(void) const
	{
		return (this->status == ResultStatus::Success);
	}

	ResultStatus getStatus(void) const
	{
		return this->status;
	}

	const char * getErrorMessage(void) const
	{
		return this->errorMessage;
	}
};

inline ResultInfo resultSuccess(void)
{
	return ResultInfo(ResultStatus::Success);
}

inline ResultInfo resultError(const char * errorMessage)
{
	return ResultInfo(ResultStatus::Error, errorMessage);
}