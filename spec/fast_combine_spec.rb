require 'spec_helper'

RSpec.describe FastCombine do
  it 'has a version number' do
    expect(FastCombine::VERSION).not_to be nil
  end

  describe '.combine' do
    subject(:result) { described_class.combine(input, mappings) }

    let(:input) { [] }
    let(:mappings) { [] }

    it { is_expected.to be_a(Array) }

    xdescribe 'integration test' do
      let(:input) do
        [
          [
            {name: 'Jane', email: 'jane@doe.org'},
            {name: 'Joe', email: 'joe@doe.org'}
          ],
          [
            [
              # user tasks
              [
                {user: 'Jane', title: 'One'},
                {user: 'Jane', title: 'Two'},
                {user: 'Joe', title: 'Three'}
              ],
              [
                # task tags
                [
                  {task: 'One', tag: 'red'},
                  {task: 'Three', tag: 'blue'}
                ]
              ]
            ]
          ]
        ]
      end

      let(:mappings) { [[:tasks, {name: :user}, [[:tags, title: :task]]]] }

      it 'merges hashes from arrays using provided join keys' do
        output = [
          {name: 'Jane', email: 'jane@doe.org', tasks: [
            {user: 'Jane', title: 'One', tags: [{task: 'One', tag: 'red'}]},
            {user: 'Jane', title: 'Two', tags: []}
          ]},
          {
            name: 'Joe', email: 'joe@doe.org', tasks: [
              {
                user: 'Joe', title: 'Three', tags: [
                  {task: 'Three', tag: 'blue'}
                ]
              }
            ]
          }
        ]
        is_expected.to eql(output)
      end
    end
  end
end
