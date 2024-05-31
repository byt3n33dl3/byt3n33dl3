# frozen_string_literal: true

require_relative '../lib/ruby_intro.rb'

describe 'Ruby intro part 1' do
  describe '#sum' do
    it 'should be defined' do
      expect { sum([1, 3, 4]) }.not_to raise_error
    end

    it 'returns correct sum [20 points]', points: 20 do
      expect(sum([1, 2, 3, 4, 5])).to be_a_kind_of Integer
      expect(sum([1, 2, 3, 4, 5])).to eq(15)
      expect(sum([1, 2, 3, 4, -5])).to eq(5)
      expect(sum([1, 2, 3, 4, -5, 5, -100])).to eq(-90)
    end

    it 'works on the empty array [10 points]', points: 10 do
      expect { sum([]) }.not_to raise_error
      expect(sum([])).to be_zero
    end
  end

  describe '#max_2_sum' do
    it 'should be defined' do
      expect { max_2_sum([1, 2, 3]) }.not_to raise_error
    end
    it 'returns the correct sum [7 points]', points: 7 do
      expect(max_2_sum([1, 2, 3, 4, 5])).to be_a_kind_of Integer
      expect(max_2_sum([1, 2, 3, 4, 5])).to eq(9)
      expect(max_2_sum([1, -2, -3, -4, -5])).to eq(-1)
    end
    it 'works even if 2 largest values are the same [3 points]', points: 3 do
      expect(max_2_sum([1, 2, 3, 3])).to eq(6)
    end
    it 'returns zero if array is empty [10 points]', points: 10 do
      expect(max_2_sum([])).to be_zero
    end
    it 'returns value of the element if just one element [10 points]', points: 10 do
      expect(max_2_sum([3])).to eq(3)
    end
  end

  describe '#sum_to_n' do
    it 'should be defined' do
      expect { sum_to_n?([1, 2, 3], 4) }.not_to raise_error
    end
    it 'returns true when any two elements sum to the second argument [30 points]', points: 30 do
      expect(sum_to_n?([1, 2, 3, 4, 5], 5)).to be true # 2 + 3 = 5
      expect(sum_to_n?([3, 0, 5], 5)).to be true # 0 + 5 = 5
      expect(sum_to_n?([-1, -2, 3, 4, 5, -8], -3)).to be true  # handles negative sum
      expect(sum_to_n?([-1, -2, 3, 4, 5, -8], 12)).to be false # 3 + 4 + 5 = 12 (not 3 elements)
      expect(sum_to_n?([-1, -2, 3, 4, 6, -8], 12)).to be false # no two elements that sum
    end
    it 'returns false for any single element array [5 points]', points: 5 do
      expect(sum_to_n?([0], 0)).to be false
      expect(sum_to_n?([1], 1)).to be false
      expect(sum_to_n?([-1], -1)).to be false
      expect(sum_to_n?([-3], 0)).to be false
    end
    it 'returns false for an empty array [5 points]', points: 5 do
      expect(sum_to_n?([], 0)).to be false
      expect(sum_to_n?([], 7)).to be false
      describe '#hello' do
  it 'should be defined' do
    expect { hello('Testing') }.not_to raise_error#::NoMethodError)
  end

  it 'The hello method returns the correct string [30 points]', points: 30 do
    expect(hello('Dan').class).to eq(String)
    expect(hello('Dan')).to eq('Hello, Dan'), 'Incorrect results for input: "Dan"'
    expect(hello('BILL')).to eq('Hello, BILL'), 'Incorrect results for input: "BILL"'
    expect(hello('Mr. Ilson')).to eq('Hello, Mr. Ilson'), 'Incorrect results for input: "Mr. Ilson"'
  end
end

describe '#starts_with_consonant?' do
  it 'should be defined' do
    expect { starts_with_consonant?('d') }.not_to raise_error#::NoMethodError)
  end
  it 'classifies true cases [10 points]' , points: 10 do
    expect(starts_with_consonant?('v')).to be_truthy, "'v' is a consonant"
    %w[v vest Veeee crypt].each do |string|
      expect(starts_with_consonant?(string)).to be_truthy, "Incorrect results for input: \"#{string}\""
    end
  end
  it 'classifies false cases [10 points]' , points: 10 do
    expect(starts_with_consonant?('a')).to be_falsy, "'a' is not a consonant"
    %w[asdfgh Unix].each do |string|
      expect(starts_with_consonant?(string)).to be_falsy, "Incorrect results for input: \"#{string}\""
    end
  end
  it 'works on the empty string [5 points]' , points: 5 do
    expect(starts_with_consonant?('')).to be_falsy
  end
  it 'works on nonletters [5 points]' , points: 5 do
    expect(starts_with_consonant?('#foo')).to be_falsy
  end
end

describe '#binary_multiple_of_4?' do
  it 'should be defined' do
    expect { binary_multiple_of_4?('yes') }.not_to raise_error#::NoMethodError)
  end
  it 'classifies valid binary numbers [30 points]' , points: 30 do
    %w[1010101010100 0101010101010100 100 0].each do |string|
      expect(binary_multiple_of_4?(string)).to be_truthy,  "Incorrect results for input: \"#{string}\""
    end
    %w[101 1000000000001].each do |string|
      expect(binary_multiple_of_4?(string)).not_to be_truthy,  "Incorrect results for input: \"#{string}\""
    end
  end
  it 'rejects invalid binary numbers [10 points]' , points: 10 do
    expect(binary_multiple_of_4?('a100')).to be_falsy, "'a100' is not a valid binary number!"
    expect(binary_multiple_of_4?('')).to be_falsy, 'The empty string is not a valid binary number!'
    end
  end
end
describe 'BookInStock' do
  it 'should be defined' do
    expect { BookInStock }.not_to raise_error
  end

  describe 'getters and setters' do
    before(:each)  { @book = BookInStock.new('isbn1', 33.8) }
    it 'should set ISBN [10 points]', points: 10 do
      expect(@book.isbn).to eq('isbn1')
    end
    it 'should set price [10 points]', points: 10 do
      expect(@book.price).to eq(33.8)
    end
    it 'should be able to change ISBN [10 points]', points: 10 do
      @book.isbn = 'isbn2'
      expect(@book.isbn).to eq('isbn2')
    end
    it 'should be able to change price [10 points]', points: 10 do
      @book.price = 300.0
      expect(@book.price).to eq(300.0)
    end
  end
  describe 'constructor' do
    it 'should reject invalid ISBN number [10 points]', points: 10 do
      expect { BookInStock.new('', 25.00) }.to raise_error(ArgumentError)
    end
    it 'should reject zero price [10 points]', points: 10 do
      expect { BookInStock.new('isbn1', 0) }.to raise_error(ArgumentError)
    end
    it 'should reject negative price [10 points]', points: 10 do
      expect { BookInStock.new('isbn1', -5.0) }.to raise_error(ArgumentError)
    end
  end
  describe '#price_as_string' do
    it 'should be defined' do
      expect(BookInStock.new('isbn1', 10)).to respond_to(:price_as_string)
    end
    it 'should display 33.95 as "$33.95" [10 points]', points: 10 do
      expect(BookInStock.new('isbn11', 33.95).price_as_string).to eq('$33.95')
    end
    it 'should display 1.1 as $1.10 [10 points]', points: 10 do
      expect(BookInStock.new('isbn11', 1.1).price_as_string).to eq('$1.10')
    end
    it 'should display 20 as $20.00 [10 points]', points: 10 do
      expect(BookInStock.new('isbn11', 20).price_as_string).to eq('$20.00')
    end
  end
end
